/************************************************************************************//**
    @file	c2xa/create_template.hpp
    @brief	Create Template

    @author    êVÅTåé(NewNotMoon)
    @date      2015/08/29
****************************************************************************************/
#ifndef C2XA_CREATE_TEMPLATE_HPP
#define C2XA_CREATE_TEMPLATE_HPP

namespace c2xa
{
    template< typename TYPE, typename... ARGS >
    inline TYPE* create_template( ARGS&&... a )
    {
        auto p = new TYPE{};

        if( p && p->init( std::forward<ARGS>( a )... ) )
        {
            p->autorelease();
            return p;
        }
        else
        {
            delete  p;
            p = nullptr;
            return nullptr;
        }
    };
}

#endif//C2XA_CREATE_TEMPLATE_HPP