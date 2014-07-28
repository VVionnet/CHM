
#include "module_factory.hpp"



module_base* module_factory::get(std::string ID)
{
        LOG_DEBUG << "Module ID=" << ID;
        
        module_base* mod = NULL;
        if (ID == "solar")
            mod = new Solar(ID);
        else if (ID == "shadows")
            mod = new terrain_shadow(ID);
        else
            mod =  NULL;
        
        if(mod == NULL)
        {
            BOOST_THROW_EXCEPTION(module_not_found() 
                                  << errstr_info( std::string("Module not found ") + ID)
                            );
        }
        else
        {
            return mod;
        }
}
