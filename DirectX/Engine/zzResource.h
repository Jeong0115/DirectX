#pragma once

#include "zzEntity.h"

namespace zz
{
	class Resource : public Entity
	{
	public:
		Resource(const eResourceType type);
		virtual ~Resource();

	public:
		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetKey() { return mKey; }
		const std::wstring& GetKPath() { return mPath; }

		void SetKey(const std::wstring& key) { mKey = key; }
		void SetPath(const std::wstring& path) { mPath = path; }


	private:
		std::wstring    mKey;
		std::wstring    mPath;

        eResourceType   mType;
	};
}

