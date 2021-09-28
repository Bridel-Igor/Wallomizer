#pragma once

namespace Filesystem
{
	void initialize();
	void getRoamingDir(wchar_t* path);
	void getRoamingDirNative(wchar_t* path);
}