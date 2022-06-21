#include <ntdef.h>
#include <ntifs.h>
#include <ntimage.h>
#include <windef.h>

#include "definitions.h"
#include "utils.h"
#include "eac.h"

// https://www.unknowncheats.me/forum/anti-cheat-bypass/503052-easy-anti-cheat-kernel-packet-fucker.html
auto DriverEntry( ) -> NTSTATUS
{
	/*
	make this run when eac driver is loaded little paster
	*/

	const auto easy_anti_cheat = utils::get_kernel_module( "EasyAntiCheat.sys" );

	if ( !easy_anti_cheat.base )
	{
		DbgPrintEx( 0, 0, "EasyAntiCheat.sys is not loaded." );
		return STATUS_UNSUCCESSFUL;
	}

	DbgPrintEx( 0, 0, "found EasyAntiCheat.sys: 0x%llx with size of 0x%llx", easy_anti_cheat.base, easy_anti_cheat.size );
	
	if ( eac::hook_allocation( easy_anti_cheat.base, easy_anti_cheat.size ) )
	{
		DbgPrintEx( 0, 0, "successfully hooked EasyAntiCheat's allocation" );
	}
	else
	{
		DbgPrintEx( 0, 0, "failed hooking EasyAntiCheat's allocation" );
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}
