/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include "gatewaystruct.hxx"
#include "string.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createStringMatrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, "createStringMatrix", _("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, "createStringMatrix", _("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::String* s = new types::String(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, "createStringMatrix", _("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createStringMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, "createStringMatrix2d", _("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::String* s = new types::String(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, "createStringMatrix2d", _("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createString)(scilabEnv env, const char* val)
{
    types::String* s = new types::String(val);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, "createString", _("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabStatus API_PROTO(getStringArray)(scilabEnv env, scilabVar var, char const* const** strs)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false)
    {
        scilab_setInternalError(env, "getStringArray", _("var must be a string variable"));
        return STATUS_ERROR;
    }
#endif
    *strs = s->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getString)(scilabEnv env, scilabVar var, char const** str)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false || s->isScalar() == false)
    {
        scilab_setInternalError(env, "getString", _("var must be a scalar string variable"));
        return STATUS_ERROR;
    }
#endif
    *str = s->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(setStringArray)(scilabEnv env, scilabVar var, const char* const* strs)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false)
    {
        scilab_setInternalError(env, "setStringArray", _("var must be a string variable"));
        return STATUS_ERROR;
    }
#endif
    s->set(strs);
    return STATUS_OK;
}

scilabStatus API_PROTO(setString)(scilabEnv env, scilabVar var, char* str)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false || s->isScalar() == false)
    {
        scilab_setInternalError(env, "setString", _("var must be a scalar string variable"));
        return STATUS_ERROR;
    }
#endif
    s->set(0, str);
    return STATUS_OK;
}