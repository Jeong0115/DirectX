#pragma once

#include <algorithm>
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <queue>
#include <future>
#include <deque>
#include <condition_variable>
#include <type_traits>

namespace zz
{
    class ThreadPool
    {
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::condition_variable completed;
        std::atomic<int> active;
        bool stop;
    public:
        ThreadPool(size_t threads) : stop(false), active(0) {
            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back(
                    [this] {
                        for (;;) {
                            std::function<void()> task;
                            {
                                std::unique_lock<std::mutex> lock(this->queue_mutex);
                                this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                                if (this->stop && this->tasks.empty()) return;
                                task = std::move(this->tasks.front());
                                ++active;
                                this->tasks.pop();
                            }
                            
                            task();
                            --active;
                            completed.notify_one();
                        }
                    }
                );
            }
        }

        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
            using return_type = typename std::invoke_result<F, Args...>::type;
            auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
                tasks.emplace([task]() { (*task)(); });
            }
            condition.notify_one();
            return res;
        }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread& worker : workers) worker.join();
        }

        void wait() {
            std::unique_lock<std::mutex> lock(queue_mutex);
            completed.wait(lock, [this]() { return tasks.empty() && (active == 0); });
        }

    
    };

    //template<typename _t>
    //class blocking_queue {
    //public:
    //    using iterator = typename std::deque<_t>::iterator;
    //    using const_iterator = typename std::deque<_t>::const_iterator;
    //private:
    //    std::mutex              m_mutex;
    //    std::condition_variable m_condition;
    //    std::deque<_t>          m_queue;

    //public:
    //    /**
    //    * Pushes an item to the back of the queue.
    //    *
    //    * @param item The item to push.
    //    */
    //    void push(
    //        const _t& item)
    //    {
    //        std::unique_lock<std::mutex> lock(m_mutex);
    //        m_queue.push_back(item);

    //        //lock.unlock();
    //        m_condition.notify_one();
    //    }

    //    /**
    //    * Pops an item from the front of the queue.
    //    */
    //    _t pop() {
    //        std::unique_lock<std::mutex> lock(m_mutex);
    //        m_condition.wait(lock, [=] { return !m_queue.empty(); });

    //        _t item = m_queue.front(); m_queue.pop_front();
    //        return item;
    //    }

    //    void clear() {
    //        std::unique_lock<std::mutex> lock(m_mutex);
    //        m_queue.clear();

    //        //lock.unlock();
    //        m_condition.notify_one();
    //    }

    //    bool empty() {
    //        return m_queue.empty(); // not sure if this is thread safe?
    //    }

    //    size_t size() {
    //        return m_queue.size();
    //    }

    //    iterator begin() {
    //        return m_queue.begin();
    //    }

    //    iterator end() {
    //        return m_queue.end();
    //    }

    //    const_iterator begin() const {
    //        return m_queue.begin();
    //    }

    //    const_iterator end() const {
    //        return m_queue.end();
    //    }
    //};

    //class thread_pool {
    //private:
    //    struct func_t {
    //        std::function<void()> func;
    //        //std::string name;
    //        bool poison = false;
    //    };

    //    using my_time = long long;

    //    my_time get_time() {
    //        using namespace std::chrono;
    //        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    //    }

    //    std::vector<std::thread> m_pool;
    //    blocking_queue<func_t> m_queue;

    //    std::mutex m_mutex_coroutine;
    //    std::vector<                           std::function<bool()>>  m_coroutines; // main thread tasks
    //    std::vector<std::tuple<my_time, float, std::function<void()>>> m_defered;    // ^

    //public:
    //    thread_pool(
    //        unsigned threads)
    //        //: m_poison(detail::poison)
    //    {
    //        m_pool.resize(threads);
    //    }

    //    ~thread_pool() {
    //        shutdown();
    //    }

    //    void init() {
    //        for (std::thread& thread : m_pool) {
    //            thread = std::thread([&]() {
    //                while (true) {
    //                    func_t/*&*/ func = m_queue.pop(); // was reference the problem?

    //                    if (func.poison) {
    //                        break;
    //                    }

    //                    //LOG_DEBUG << "Called function " << func.name;

    //                    if (!func.func) {
    //                        OutputDebugStringW(L"Tried to call queued function in thread pool but it was missing!");
    //                    }

    //                    func.func();
    //                }
    //                });
    //        }
    //    }

    //    void shutdown() {
    //        for (std::thread& thread : m_pool) {
    //            func_t t{
    //                {},
    //                //"",
    //                true // stop thread
    //            };

    //            m_queue.push(t);
    //        }

    //        for (std::thread& thread : m_pool) {
    //            thread.join();
    //        }
    //    }

    //    void queue(
    //        std::function<void()> func/*,
    //        std::string& name*/)
    //    {
    //        func_t t{
    //            func,
    //            //name,
    //            false
    //        };

    //        m_queue.push(t);
    //    }

    //    void step_coroutines()
    //    {
    //        // copy to local because tasks could add more

    //        std::vector<                           std::function<bool()>>  copy_coroutines;
    //        std::vector<std::tuple<my_time, float, std::function<void()>>> copy_defered;
    //        {
    //            std::unique_lock lock(m_mutex_coroutine);
    //            copy_coroutines = m_coroutines;
    //            copy_defered = m_defered;
    //        }

    //        for (size_t i = 0; i < copy_coroutines.size(); i++)
    //        {
    //            if (copy_coroutines.at(i)())
    //            {
    //                copy_coroutines.erase(copy_coroutines.begin() + i); // sucks, lots of moves
    //            }
    //        }

    //        for (size_t i = 0; i < copy_defered.size(); i++)
    //        {
    //            auto& [begin, delay, func] = copy_defered.at(i);

    //            if (get_time() - begin > (my_time)(delay * 1000.f))
    //            {
    //                func();
    //                copy_defered.erase(copy_defered.begin() + i);
    //            }
    //        }

    //        // move back
    //        {
    //            std::unique_lock lock(m_mutex_coroutine);
    //            m_coroutines = std::move(copy_coroutines);
    //            m_defered = std::move(copy_defered);
    //        }
    //    }

    //    void coroutine(
    //        std::function<bool()> func)
    //    {
    //        std::unique_lock lock(m_mutex_coroutine);
    //        m_coroutines.push_back(func);
    //    }

    //    void defer(
    //        std::function<void()> func)
    //    {
    //        delay(-1.f, func);
    //    }

    //    void delay(
    //        float howLong,
    //        std::function<void()> func)
    //    {
    //        std::unique_lock lock(m_mutex_coroutine);
    //        m_defered.emplace_back(get_time(), howLong, func);
    //    }

    //    template<
    //        typename _container,
    //        typename _func>
    //    void foreach(
    //        _container& container,
    //        _func&& func)
    //    {
    //        std::mutex mutex;
    //        std::condition_variable cond;

    //        size_t remaining = container.size();

    //        for (auto index = 0; index < container.size(); index++)
    //        {
    //            queue([&, func, index]() // need to copy func?
    //                {
    //                    func(index);

    //                    std::unique_lock lock(mutex);
    //                    remaining -= 1;
    //                    cond.notify_one();
    //                });
    //        }

    //        std::unique_lock lock(mutex);
    //        cond.wait(lock, [&]() { return remaining == 0; });
    //    }

    //    template<
    //        typename _container,
    //        typename _preThreadFunc,
    //        typename _func>
    //    void foreach(
    //        _container& container,
    //        _preThreadFunc&& preThreadFunc,
    //        _func&& func)
    //    {
    //        std::mutex mutex;
    //        std::condition_variable cond;

    //        size_t remaining = container.size();
    //        auto index = 0;

    //        for (auto& element : container)
    //        {
    //            auto preThread = preThreadFunc(element);

    //            queue([&, func, index, preThread]() // need to copy func?
    //                {
    //                    func(index, preThread);

    //                    std::unique_lock lock(mutex);
    //                    remaining -= 1;
    //                    cond.notify_one();
    //                });

    //            index += 1;
    //        }

    //        std::unique_lock lock(mutex);
    //        cond.wait(lock, [&]() { return remaining == 0; });
    //    }
    //};
}

