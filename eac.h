namespace eac
{
	typedef __int64( __fastcall *ex_allocate_pool_with_tag_t )( INT64, size_t, uintptr_t );
	ex_allocate_pool_with_tag_t ex_allocate_pool_with_tag_fn = nullptr;

	auto hk_allocation( INT64 type, size_t size, uintptr_t tag ) -> __int64 __fastcall
	{
		if ( size == 33096 + 16 ) // 16 size of a packet
		{
			DbgPrintEx( 0, 0, "EasyAntiCheat Hydra packet denied" );
			return 0;
		}

		return ex_allocate_pool_with_tag_fn( type, size, tag );
	}

	auto hook_allocation( const uintptr_t base, const size_t size ) -> bool
	{
		if ( size <= 0x1000 )
		{
			return false;
		}
		
		ex_allocate_pool_with_tag_fn = *reinterpret_cast< ex_allocate_pool_with_tag_t* >( base + 0xD7B80 );
		*reinterpret_cast<uintptr_t*>( base + 0xD7B80 ) = reinterpret_cast<uintptr_t>( &hk_allocation );

		return true;
	}
}
