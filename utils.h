namespace utils
{
    auto get_system_information( const SYSTEM_INFORMATION_CLASS information_class ) -> const void *
    {
        unsigned long size = 32;
        char buffer[32];

        ZwQuerySystemInformation( information_class, buffer, size, &size );

        void *info = ExAllocatePoolZero( NonPagedPool, size, 7265746172 );

        if ( !info )
        {
            return nullptr;
        }

        if ( ZwQuerySystemInformation( information_class, info, size, &size ) != STATUS_SUCCESS )
        {
            ExFreePool( info );
            return nullptr;
        }

        return info;
    }

    typedef struct _image_t
    {
        uintptr_t base;
        size_t size;
    }image_t, *pimage_t;

    auto get_kernel_module( const char *name ) -> const image_t
    {
        const auto info = ( RTL_PROCESS_MODULES * )get_system_information( SystemModuleInformation );

        if ( !info )
        {
            return { 0, 0 };
        }

        for ( size_t i = 0; i < info->NumberOfModules; ++i )
        {
            const auto &mod = info->Modules[i];

            if ( strcmp( to_lower( ( char * )mod.FullPathName + mod.OffsetToFileName ), name ) == 0 )
            {
                ExFreePool( info );
                
                return
                {
                    reinterpret_cast< uintptr_t > ( mod.ImageBase ),
                    reinterpret_cast< size_t> ( mod.ImageBase ) 
                };
            }
        }

        ExFreePool( info );
        return { 0, 0 };
    }
}