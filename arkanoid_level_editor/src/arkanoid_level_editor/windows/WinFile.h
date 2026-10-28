#pragma once

#include <vector>
#include <shtypes.h>

#include "arkanoid_lib/resources/FilePCH.h"

#if defined(CreateDirectory)
#undef CreateDirectory
#undef CreateDirectoryA
#undef CreateDirectoryW
#endif

namespace arkanoid::windows
{
	bool PickFile(fs::path& a_sPath, const std::vector<COMDLG_FILTERSPEC>& a_aFilters = {}, const fs::path& a_InitialPath = {});

	bool SaveFile(fs::path& a_sPath, const std::vector<COMDLG_FILTERSPEC>& a_aFilters = {}, const fs::path& a_InitialPath = {});
}