/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/lua/CLuaFunctionDefs.Projectile.cpp
*  PURPOSE:     Lua function definitions class
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*               Jax <>
*               Cecill Etheredge <ijsf@gmx.net>
*               Kevin Whiteside <kevuwk@gmail.com>
*               Chris McArthur <>
*               Derek Abdine <>
*               Christian Myhre Lundheim <>
*               Stanislav Bobrov <lil_toady@hotmail.com>
*               Alberto Alonso <rydencillo@gmail.com>
*
*****************************************************************************/

#include "StdInc.h"

int CLuaFunctionDefs::CreateProjectile ( lua_State* luaVM )
{
    CVector vecOrigin;
    CClientEntity* pCreator = NULL;
    CClientEntity* pTemp = NULL;
    unsigned char ucWeaponType = 0;
    CScriptArgReader argStream ( luaVM );
    float fForce = 1.0f;
    CClientEntity* pTarget = NULL;
    CVector *pvecRotation = new CVector ( 0, 0, 0 ), *pvecMoveSpeed = new CVector ( 0, 0, 0 );
    unsigned short usModel = 0;
    argStream.ReadUserData ( pCreator );
    if ( pCreator )
        pCreator->GetPosition ( vecOrigin );

    argStream.ReadNumber ( ucWeaponType );
    argStream.ReadNumber ( vecOrigin.fX, vecOrigin.fX );
    argStream.ReadNumber ( vecOrigin.fY, vecOrigin.fY );
    argStream.ReadNumber ( vecOrigin.fZ, vecOrigin.fZ );
    argStream.ReadNumber ( fForce, 1.0f );
    argStream.ReadUserData ( pTemp, NULL );
    argStream.ReadNumber ( pvecRotation->fX, pvecRotation->fX );
    argStream.ReadNumber ( pvecRotation->fY, pvecRotation->fY );
    argStream.ReadNumber ( pvecRotation->fZ, pvecRotation->fZ );
    argStream.ReadNumber ( pvecMoveSpeed->fX, pvecMoveSpeed->fX );
    argStream.ReadNumber ( pvecMoveSpeed->fY, pvecMoveSpeed->fY );
    argStream.ReadNumber ( pvecMoveSpeed->fZ, pvecMoveSpeed->fZ );
    argStream.ReadNumber ( usModel, 0 );

    if ( !argStream.HasErrors ( ) )
    {
        if ( pCreator )
        {
            CLuaMain* pLuaMain = m_pLuaManager->GetVirtualMachine ( luaVM );
            if ( pLuaMain )
            {
                CResource * pResource = pLuaMain->GetResource();
                if ( pResource )
                {
                    CClientProjectile * pProjectile = CStaticFunctionDefinitions::CreateProjectile ( *pResource, *pCreator, ucWeaponType, vecOrigin, fForce, pTarget, pvecRotation, pvecMoveSpeed, usModel );
                    if ( pProjectile )
                    {
                        CElementGroup * pGroup = pResource->GetElementGroup();
                        if ( pGroup )
                        {
                            pGroup->Add ( ( CClientEntity* ) pProjectile );
                        }

                        if ( pvecRotation )
                        {
                            delete pvecRotation;
                            pvecRotation = NULL;
                        }
                        if ( pvecMoveSpeed )
                        {
                            delete pvecMoveSpeed;
                            pvecMoveSpeed = NULL;
                        }

                        lua_pushelement ( luaVM, pProjectile );
                        return 1;
                    }
                }
            }
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "element", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    if ( pvecRotation )
    {
        delete pvecRotation;
        pvecRotation = NULL;
    }
    if ( pvecMoveSpeed )
    {
        delete pvecMoveSpeed;
        pvecMoveSpeed = NULL;
    }
    lua_pushboolean ( luaVM, false );
    return 1;
}

int CLuaFunctionDefs::GetProjectileType ( lua_State* luaVM )
{
    CClientProjectile* pProjectile = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pProjectile );

    if ( !argStream.HasErrors ( ) )
    {
        if ( pProjectile )
        {
            unsigned char ucWeapon = pProjectile->GetWeaponType();
            lua_pushnumber ( luaVM, static_cast < lua_Number > ( ucWeapon ) );
            return 1;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "projectile", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}

int CLuaFunctionDefs::GetProjectileTarget ( lua_State* luaVM )
{
    CClientProjectile* pProjectile = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pProjectile );

    if ( !argStream.HasErrors ( ) )
    {
        if ( pProjectile )
        {
            unsigned char ucWeapon = pProjectile->GetWeaponType ( );
            if ( ucWeapon == WEAPONTYPE_ROCKET_HS )
            {
                lua_pushelement ( luaVM, pProjectile->GetTargetEntity ( ) );
                return 1;
            }
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "projectile", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}

int CLuaFunctionDefs::GetProjectileCreator ( lua_State* luaVM )
{
    // Verify the argument
    CClientProjectile* pProjectile = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pProjectile );

    if ( !argStream.HasErrors ( ) )
    {
        if ( pProjectile )
        {
            lua_pushelement ( luaVM, pProjectile->GetCreator() );
            return 1;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "projectile", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}

int CLuaFunctionDefs::GetProjectileForce ( lua_State* luaVM )
{
    // Verify the argument
    CClientProjectile* pProjectile = NULL;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pProjectile );

    if ( !argStream.HasErrors ( ) )
    {
        if ( pProjectile )
        {
            lua_pushnumber ( luaVM, static_cast < lua_Number > ( pProjectile->GetForce() ) );
            return 1;
        }
        else
            m_pScriptDebugging->LogBadPointer ( luaVM, "projectile", 1 );
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage () );

    lua_pushboolean ( luaVM, false );
    return 1;
}


int CLuaFunctionDefs::SetProjectileCounter ( lua_State* luaVM )
{
    CClientProjectile* pProjectile = NULL;
    DWORD dwCounter = 0;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pProjectile );
    argStream.ReadNumber ( dwCounter );

    // Verify the argument
    if ( !argStream.HasErrors( ) )
    {
        pProjectile->SetCounter( dwCounter );
        lua_pushboolean ( luaVM, true );
        return 1;
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage() );

    lua_pushboolean ( luaVM, false );
    return 1;
}



int CLuaFunctionDefs::GetProjectileCounter ( lua_State* luaVM )
{
    CClientProjectile* pProjectile = NULL;
    DWORD dwCounter = 0;
    CScriptArgReader argStream ( luaVM );
    argStream.ReadUserData ( pProjectile );

    // Verify the argument
    if ( !argStream.HasErrors( ) )
    {
        dwCounter = pProjectile->GetCounter( );
        lua_pushnumber ( luaVM, dwCounter );
        return 1;
    }
    else
        m_pScriptDebugging->LogCustom ( luaVM, argStream.GetFullErrorMessage() );

    lua_pushboolean ( luaVM, false );
    return 1;
}