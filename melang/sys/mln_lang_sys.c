
/*
 * Copyright (C) Niklaus F.Schen.
 */
#include "sys/mln_lang_sys.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __DEBUG__
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x);
#endif

static int mln_lang_sys_size_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_size_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_keys_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_keys_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_keys_scanner(mln_rbtree_node_t *node, void *rn_data, void *udata);
static int mln_lang_sys_merge_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_merge_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_has_handler(mln_lang_ctx_t *ctx);
static int mln_lang_sys_merge_scanner(mln_rbtree_node_t *node, void *rn_data, void *udata);
static mln_lang_retExp_t *mln_lang_sys_has_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isInt_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isInt_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isReal_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isReal_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isStr_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isStr_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isNil_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isNil_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isBool_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isBool_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isObj_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isObj_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isFunc_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isFunc_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_isArray_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_isArray_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_int_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_int_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_bool_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_bool_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_real_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_real_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_str_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_str_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_obj_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_obj_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_obj_add_key(mln_rbtree_node_t *node, void *rn_data, void *udata);
static int mln_lang_sys_array_handler(mln_lang_ctx_t *ctx);
static mln_lang_retExp_t *mln_lang_sys_array_process(mln_lang_ctx_t *ctx);
static int mln_lang_sys_array_add_nil(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_lang_var_t *key);
static int mln_lang_sys_array_add_int(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_s64_t i, mln_lang_var_t *key);
static int mln_lang_sys_array_add_bool(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_u8_t b, mln_lang_var_t *key);
static int mln_lang_sys_array_add_real(mln_lang_ctx_t *ctx, mln_lang_array_t *array, double f, mln_lang_var_t *key);
static int mln_lang_sys_array_add_string(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_string_t *s, mln_lang_var_t *key);
static int mln_lang_sys_array_add_func(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_lang_func_detail_t *f, mln_lang_var_t *key);
static int mln_lang_sys_array_add_elem(mln_rbtree_node_t *node, void *rn_data, void *udata);

int mln_lang_sys(mln_lang_ctx_t *ctx)
{
    if (mln_lang_sys_size_handler(ctx) < 0) return -1;
    if (mln_lang_sys_keys_handler(ctx) < 0) return -1;
    if (mln_lang_sys_merge_handler(ctx) < 0) return -1;
    if (mln_lang_sys_has_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isInt_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isReal_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isStr_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isNil_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isBool_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isObj_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isFunc_handler(ctx) < 0) return -1;
    if (mln_lang_sys_isArray_handler(ctx) < 0) return -1;
    if (mln_lang_sys_int_handler(ctx) < 0) return -1;
    if (mln_lang_sys_bool_handler(ctx) < 0) return -1;
    if (mln_lang_sys_real_handler(ctx) < 0) return -1;
    if (mln_lang_sys_str_handler(ctx) < 0) return -1;
    if (mln_lang_sys_obj_handler(ctx) < 0) return -1;
    if (mln_lang_sys_array_handler(ctx) < 0) return -1;
    return 0;
}

static int mln_lang_sys_array_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_array");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_array_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_array_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type == M_LANG_SYMBOL_VAR) {
        mln_s32_t type = mln_lang_var_getValType(sym->data.var);
        mln_lang_val_t *val = mln_lang_var_getVal(sym->data.var);
        if ((retExp = mln_lang_retExp_createTmpArray(ctx, NULL)) == NULL) {
            mln_lang_errmsg(ctx, "No memory.");
            return NULL;
        }
        mln_lang_array_t *array = retExp->data.var->val->data.array;
        switch (type) {
            case M_LANG_VAL_TYPE_NIL:
                if (mln_lang_sys_array_add_nil(ctx, array, NULL) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_INT:
                if (mln_lang_sys_array_add_int(ctx, array, val->data.i, NULL) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_BOOL:
                if (mln_lang_sys_array_add_bool(ctx, array, val->data.b, NULL) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_REAL:
                if (mln_lang_sys_array_add_real(ctx, array, val->data.f, NULL) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_STRING:
                if (mln_lang_sys_array_add_string(ctx, array, val->data.s, NULL) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_OBJECT:
                if (mln_rbtree_scan_all(val->data.obj->members, mln_lang_sys_array_add_elem, array) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_FUNC:
                if (mln_lang_sys_array_add_func(ctx, array, val->data.func, NULL) < 0) {
                    mln_lang_retExp_free(retExp);
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_ARRAY:
            {
                mln_lang_retExp_free(retExp);
                mln_lang_var_t *var;
                if ((var = mln_lang_var_dup(ctx, sym->data.var)) == NULL) {
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                if ((retExp = mln_lang_retExp_new(ctx->pool, M_LANG_RETEXP_VAR, var)) == NULL) {
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            }
            default:
                ASSERT(0);
                mln_lang_retExp_free(retExp);
                mln_lang_errmsg(ctx, "Cannot convert to ARRAY.");
                return NULL;
        }
        return retExp;
    }
    mln_lang_errmsg(ctx, "Cannot convert to ARRAY.");
    return NULL;
}

static int mln_lang_sys_array_add_nil(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_lang_var_t *key)
{
    return mln_lang_array_getAndNew(ctx, array, key) == NULL ? -1 : 0;
}

static int mln_lang_sys_array_add_int(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_s64_t i, mln_lang_var_t *key)
{
    mln_lang_var_t *array_val, var;
    mln_lang_val_t val;
    if ((array_val = mln_lang_array_getAndNew(ctx, array, key)) == NULL) {
         return -1;
    }
    var.type = M_LANG_VAR_NORMAL;
    var.name = NULL;
    var.val = &val;
    var.inSet = NULL;
    var.prev = var.next = NULL;
    val.data.i = i;
    val.type = M_LANG_VAL_TYPE_INT;
    val.ref = 1;
    if (mln_lang_var_setValue(ctx, array_val, &var) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_array_add_bool(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_u8_t b, mln_lang_var_t *key)
{
    mln_lang_var_t *array_val, var;
    mln_lang_val_t val;
    if ((array_val = mln_lang_array_getAndNew(ctx, array, key)) == NULL) {
        return -1;
    }
    var.type = M_LANG_VAR_NORMAL;
    var.name = NULL;
    var.val = &val;
    var.inSet = NULL;
    var.prev = var.next = NULL;
    val.data.b = b;
    val.type = M_LANG_VAL_TYPE_BOOL;
    val.ref = 1;
    if (mln_lang_var_setValue(ctx, array_val, &var) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_array_add_real(mln_lang_ctx_t *ctx, mln_lang_array_t *array, double f, mln_lang_var_t *key)
{
    mln_lang_var_t *array_val, var;
    mln_lang_val_t val;
    if ((array_val = mln_lang_array_getAndNew(ctx, array, key)) == NULL) {
         return -1;
    }
    var.type = M_LANG_VAR_NORMAL;
    var.name = NULL;
    var.val = &val;
    var.inSet = NULL;
    var.prev = var.next = NULL;
    val.data.f = f;
    val.type = M_LANG_VAL_TYPE_REAL;
    val.ref = 1;
    if (mln_lang_var_setValue(ctx, array_val, &var) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_array_add_string(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_string_t *s, mln_lang_var_t *key)
{
    mln_lang_var_t *array_val, var;
    mln_lang_val_t val;
    if ((array_val = mln_lang_array_getAndNew(ctx, array, key)) == NULL) {
        return -1;
    }
    var.type = M_LANG_VAR_NORMAL;
    var.name = NULL;
    var.val = &val;
    var.inSet = NULL;
    var.prev = var.next = NULL;
    val.data.s = s;
    val.type = M_LANG_VAL_TYPE_STRING;
    val.ref = 1;
    if (mln_lang_var_setValue(ctx, array_val, &var) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_array_add_func(mln_lang_ctx_t *ctx, mln_lang_array_t *array, mln_lang_func_detail_t *f, mln_lang_var_t *key)
{
    mln_lang_var_t *array_val, var;
    mln_lang_val_t val;
    if ((array_val = mln_lang_array_getAndNew(ctx, array, key)) == NULL) {
        return -1;
    }
    var.type = M_LANG_VAR_NORMAL;
    var.name = NULL;
    var.val = &val;
    var.inSet = NULL;
    var.prev = var.next = NULL;
    val.data.func = f;
    val.type = M_LANG_VAL_TYPE_FUNC;
    val.ref = 1;
    if (mln_lang_var_setValue(ctx, array_val, &var) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_array_add_elem(mln_rbtree_node_t *node, void *rn_data, void *udata)
{
    mln_lang_array_t *array = (mln_lang_array_t *)udata;
    mln_lang_ctx_t *ctx = array->ctx;
    mln_lang_var_t *prop = (mln_lang_var_t *)rn_data;
    ASSERT(prop->name != NULL);
    mln_lang_var_t *array_val, var;
    mln_lang_val_t val;
    var.type = M_LANG_VAR_NORMAL;
    var.name = NULL;
    var.val = &val;
    var.inSet = NULL;
    var.prev = var.next = NULL;
    val.data.s = prop->name;
    val.type = M_LANG_VAL_TYPE_STRING;
    val.ref = 1;
    if ((array_val = mln_lang_array_getAndNew(ctx, array, &var)) == NULL) {
        return -1;
    }
    if (mln_lang_var_setValue(ctx, array_val, prop) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_obj_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_obj");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_obj_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_obj_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type == M_LANG_SYMBOL_VAR) {
        mln_s32_t type = mln_lang_var_getValType(sym->data.var);
        if (type == M_LANG_VAL_TYPE_OBJECT) {
            mln_lang_var_t *var;
            if ((var = mln_lang_var_dup(ctx, sym->data.var)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            if ((retExp = mln_lang_retExp_new(ctx->pool, M_LANG_RETEXP_VAR, var)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        } else if (type == M_LANG_VAL_TYPE_ARRAY) {
            mln_lang_val_t *val = mln_lang_var_getVal(sym->data.var);
            mln_lang_array_t *array = val->data.array;
            if ((retExp = mln_lang_retExp_createTmpObj(ctx, NULL, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            if (mln_rbtree_scan_all(array->elems_key, \
                                    mln_lang_sys_obj_add_key, \
                                    retExp->data.var->val->data.obj) < 0) {
                mln_lang_retExp_free(retExp);
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        } else {
            mln_lang_errmsg(ctx, "Cannot convert to OBJECT.");
            return NULL;
        }
    }
    mln_lang_errmsg(ctx, "Cannot convert to OBJECT.");
    return NULL;
}

static int mln_lang_sys_obj_add_key(mln_rbtree_node_t *node, void *rn_data, void *udata)
{
    mln_lang_var_t tmp;
    mln_lang_object_t *obj = (mln_lang_object_t *)udata;
    mln_lang_ctx_t *ctx = obj->ctx;
    mln_lang_array_elem_t *ae = (mln_lang_array_elem_t *)rn_data;
    if (mln_lang_var_getValType(ae->key) != M_LANG_VAL_TYPE_STRING) {
        return 0;
    }
    tmp.type = M_LANG_VAR_NORMAL;
    tmp.name = ae->key->val->data.s;
    tmp.val = ae->value->val;
    tmp.inSet = NULL;
    tmp.prev = NULL;
    tmp.next = NULL;
    return mln_lang_object_add_member(ctx, obj, &tmp);
}

static int mln_lang_sys_str_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_str");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_str_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_str_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type == M_LANG_SYMBOL_VAR) {
        mln_s32_t type = mln_lang_var_getValType(sym->data.var);
        mln_lang_val_t *val = mln_lang_var_getVal(sym->data.var);
        mln_size_t n = 0;
        char buf[1024] = {0};
        mln_string_t s;
        switch (type) {
            case M_LANG_VAL_TYPE_INT:
#if defined(i386) || defined(MLN_ARM32)
                n = snprintf(buf, sizeof(buf)-1, "%lld", val->data.i);
#else
                n = snprintf(buf, sizeof(buf)-1, "%ld", val->data.i);
#endif
                mln_string_nSet(&s, buf, n);
                if ((retExp = mln_lang_retExp_createTmpString(ctx->pool, &s, NULL)) == NULL) {
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_BOOL:
                if (mln_lang_condition_isTrue(sym->data.var)) {
                    n = snprintf(buf, sizeof(buf)-1, "True");
                    mln_string_nSet(&s, buf, n);
                    if ((retExp = mln_lang_retExp_createTmpString(ctx->pool, &s, NULL)) == NULL) {
                        mln_lang_errmsg(ctx, "No memory.");
                        return NULL;
                    }
                } else {
                    n = snprintf(buf, sizeof(buf)-1, "False");
                    mln_string_nSet(&s, buf, n);
                    if ((retExp = mln_lang_retExp_createTmpString(ctx->pool, &s, NULL)) == NULL) {
                        mln_lang_errmsg(ctx, "No memory.");
                        return NULL;
                    }
                }
                break;
            case M_LANG_VAL_TYPE_REAL:
                n = snprintf(buf, sizeof(buf)-1, "%lf", val->data.f);
                mln_string_nSet(&s, buf, n);
                if ((retExp = mln_lang_retExp_createTmpString(ctx->pool, &s, NULL)) == NULL) {
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            case M_LANG_VAL_TYPE_STRING:
                if ((retExp = mln_lang_retExp_createTmpString(ctx->pool, val->data.s, NULL)) == NULL) {
                    mln_lang_errmsg(ctx, "No memory.");
                    return NULL;
                }
                break;
            default:
                mln_lang_errmsg(ctx, "Cannot convert to STRING.");
                return NULL;
        }
        return retExp;
    }
    mln_lang_errmsg(ctx, "Cannot convert to STRING.");
    return NULL;
}

static int mln_lang_sys_real_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_real");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_real_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_real_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type == M_LANG_SYMBOL_VAR) {
        mln_s32_t type;
        mln_lang_val_t *val = mln_lang_var_getVal(sym->data.var);
        if ((type = mln_lang_var_getValType(sym->data.var)) == M_LANG_VAL_TYPE_INT) {
            if ((retExp = mln_lang_retExp_createTmpReal(ctx->pool, val->data.i, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        } else if (type == M_LANG_VAL_TYPE_REAL) {
            if ((retExp = mln_lang_retExp_createTmpReal(ctx->pool, val->data.f, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        } else if (type == M_LANG_VAL_TYPE_STRING) {
            double f;
            mln_s8ptr_t buf;
            if ((buf = (mln_s8ptr_t)malloc(val->data.s->len+1)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            memcpy(buf, val->data.s->data, val->data.s->len);
            buf[val->data.s->len] = 0;
            f = atof(buf);
            free(buf);
            if ((retExp = mln_lang_retExp_createTmpReal(ctx->pool, f, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        }
    }
    mln_lang_errmsg(ctx, "Cannot convert to REAL.");
    return NULL;
}

static int mln_lang_sys_bool_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_bool");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_bool_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_bool_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type == M_LANG_SYMBOL_VAR) {
        if (mln_lang_condition_isTrue(sym->data.var)) {
            retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
        } else {
            retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
        }
        if (retExp == NULL) {
            mln_lang_errmsg(ctx, "No memory.");
            return NULL;
        }
        return retExp;
    }
    mln_lang_errmsg(ctx, "Cannot convert to BOOLEAN.");
    return NULL;
}

static int mln_lang_sys_int_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_int");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_int_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_int_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type == M_LANG_SYMBOL_VAR) {
        mln_s32_t type;
        mln_lang_val_t *val = mln_lang_var_getVal(sym->data.var);
        if ((type = mln_lang_var_getValType(sym->data.var)) == M_LANG_VAL_TYPE_INT) {
            if ((retExp = mln_lang_retExp_createTmpInt(ctx->pool, val->data.i, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        } else if (type == M_LANG_VAL_TYPE_REAL) {
            if ((retExp = mln_lang_retExp_createTmpInt(ctx->pool, val->data.f, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        } else if (type == M_LANG_VAL_TYPE_STRING) {
            mln_s64_t i;
            mln_s8ptr_t buf;
            if ((buf = (mln_s8ptr_t)malloc(val->data.s->len+1)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            memcpy(buf, val->data.s->data, val->data.s->len);
            buf[val->data.s->len] = 0;
#if defined(i386) || defined(MLN_ARM32)
            i = atoll(buf);
#else
            i = atol(buf);
#endif
            free(buf);
            if ((retExp = mln_lang_retExp_createTmpInt(ctx->pool, i, NULL)) == NULL) {
                mln_lang_errmsg(ctx, "No memory.");
                return NULL;
            }
            return retExp;
        }
    }
    mln_lang_errmsg(ctx, "Cannot convert to INT.");
    return NULL;
}

static int mln_lang_sys_isInt_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isInt");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isInt_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isInt_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_INT)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isReal_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isReal");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isReal_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isReal_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_REAL)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isStr_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isStr");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isStr_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isStr_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_STRING)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isNil_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isNil");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isNil_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isNil_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_NIL)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isBool_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isBool");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isBool_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isBool_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_BOOL)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isObj_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isObj");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isObj_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isObj_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_OBJECT)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isFunc_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isFunc");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isFunc_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isFunc_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_FUNC)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_isArray_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_isArray");
    mln_string_t v1 = mln_string("var");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_isArray_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_isArray_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("var");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_ARRAY)
    {
        retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
    } else {
        retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_size_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_size");
    mln_string_t v1 = mln_string("array");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_size_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_size_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp;
    mln_string_t v1 = mln_string("array");
    mln_lang_symbolNode_t *sym;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || \
        mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_ARRAY)
    {
        mln_lang_errmsg(ctx, "Invalid type of argument 1.");
        return NULL;
    }
    if ((retExp = mln_lang_retExp_createTmpInt(ctx->pool, \
                                               sym->data.var->val->data.array->elems_index->nr_node, \
                                               NULL)) == NULL)
    {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_has_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_has");
    mln_string_t v1 = mln_string("owner");
    mln_string_t v2 = mln_string("thing");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_has_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v2, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_has_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("owner");
    mln_string_t v2 = mln_string("thing");
    mln_lang_symbolNode_t *sym, *sym2;
    mln_s32_t type;

    if ((sym2 = mln_lang_symbolNode_search(ctx, &v2, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument 2 missing.");
        return NULL;
    }
    if (sym2->type != M_LANG_SYMBOL_VAR) {
        mln_lang_errmsg(ctx, "Invalid type of argument 2.");
        return NULL;
    }
    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument 1 missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR) {
        mln_lang_errmsg(ctx, "Invalid type of argument 1.");
        return NULL;
    }
    type = mln_lang_var_getValType(sym->data.var);
    if (type == M_LANG_VAL_TYPE_ARRAY) {
        if (mln_lang_array_elem_exist(mln_lang_var_getVal(sym->data.var)->data.array, sym2->data.var)) {
            retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
        } else {
            retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
        }
    } else if (type == M_LANG_VAL_TYPE_OBJECT) {
        if (mln_lang_var_getValType(sym2->data.var) != M_LANG_VAL_TYPE_STRING) {
            mln_lang_errmsg(ctx, "Invalid type of argument 2.");
            return NULL;
        }
        if (mln_lang_set_member_search(mln_lang_var_getVal(sym->data.var)->data.obj->members, \
                                       mln_lang_var_getVal(sym2->data.var)->data.s) == NULL)
        {
            retExp = mln_lang_retExp_createTmpFalse(ctx->pool, NULL);
        } else {
            retExp = mln_lang_retExp_createTmpTrue(ctx->pool, NULL);
        }
    } else {
        mln_lang_errmsg(ctx, "Invalid type of argument 1.");
        return NULL;
    }
    if (retExp == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_keys_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_keys");
    mln_string_t v1 = mln_string("array");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_keys_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_keys_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("array");
    mln_lang_symbolNode_t *sym;
    mln_lang_array_t *array, *arr;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument 1 missing.");
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_ARRAY) {
        mln_lang_errmsg(ctx, "Invalid type of argument 1.");
        return NULL;
    }
    array = mln_lang_var_getVal(sym->data.var)->data.array;
    if ((retExp = mln_lang_retExp_createTmpArray(ctx, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    arr = retExp->data.var->val->data.array;
    if (mln_rbtree_scan_all(array->elems_key, mln_lang_sys_keys_scanner, arr) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_keys_scanner(mln_rbtree_node_t *node, void *rn_data, void *udata)
{
    mln_lang_array_t *array = (mln_lang_array_t *)udata;
    mln_lang_array_elem_t *elem = (mln_lang_array_elem_t *)rn_data;
    mln_lang_var_t *var;

    if ((var = mln_lang_array_getAndNew(array->ctx, array, NULL)) == NULL) {
        return -1;
    }
    if (mln_lang_var_setValue(array->ctx, var, elem->key) < 0) {
        return -1;
    }
    return 0;
}

static int mln_lang_sys_merge_handler(mln_lang_ctx_t *ctx)
{
    mln_lang_val_t *val;
    mln_lang_var_t *var;
    mln_lang_func_detail_t *func;
    mln_string_t funcname = mln_string("mln_merge");
    mln_string_t v1 = mln_string("array1");
    mln_string_t v2 = mln_string("array2");
    if ((func = mln_lang_func_detail_new(ctx->pool, M_FUNC_INTERNAL, mln_lang_sys_merge_process, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return -1;
    }
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v1, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_NIL, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &v2, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        mln_lang_func_detail_free(func);
        return -1;
    }
    mln_lang_var_chain_add(&(func->args_head), &(func->args_tail), var);
    ++func->nargs;
    if ((val = mln_lang_val_new(ctx->pool, M_LANG_VAL_TYPE_FUNC, func)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_func_detail_free(func);
        return -1;
    }
    if ((var = mln_lang_var_new(ctx->pool, &funcname, M_LANG_VAR_NORMAL, val, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_val_free(val);
        return -1;
    }
    if (mln_lang_symbolNode_join(ctx, M_LANG_SYMBOL_VAR, var) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_var_free(var);
        return -1;
    }
    return 0;
}

static mln_lang_retExp_t *mln_lang_sys_merge_process(mln_lang_ctx_t *ctx)
{
    mln_lang_retExp_t *retExp = NULL;
    mln_string_t v1 = mln_string("array1");
    mln_string_t v2 = mln_string("array2");
    mln_lang_symbolNode_t *sym;
    mln_lang_array_t *array, *arr;

    if ((retExp = mln_lang_retExp_createTmpArray(ctx, NULL)) == NULL) {
        mln_lang_errmsg(ctx, "No memory.");
        return NULL;
    }
    arr = retExp->data.var->val->data.array;

    if ((sym = mln_lang_symbolNode_search(ctx, &v1, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument 1 missing.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_ARRAY) {
        mln_lang_errmsg(ctx, "Invalid type of argument 1.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }
    array = mln_lang_var_getVal(sym->data.var)->data.array;
    if (mln_rbtree_scan_all(array->elems_index, mln_lang_sys_merge_scanner, arr) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }

    if ((sym = mln_lang_symbolNode_search(ctx, &v2, 1)) == NULL) {
        ASSERT(0);
        mln_lang_errmsg(ctx, "Argument 2 missing.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }
    if (sym->type != M_LANG_SYMBOL_VAR || mln_lang_var_getValType(sym->data.var) != M_LANG_VAL_TYPE_ARRAY) {
        mln_lang_errmsg(ctx, "Invalid type of argument 2.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }
    array = mln_lang_var_getVal(sym->data.var)->data.array;
    if (mln_rbtree_scan_all(array->elems_index, mln_lang_sys_merge_scanner, arr) < 0) {
        mln_lang_errmsg(ctx, "No memory.");
        mln_lang_retExp_free(retExp);
        return NULL;
    }
    return retExp;
}

static int mln_lang_sys_merge_scanner(mln_rbtree_node_t *node, void *rn_data, void *udata)
{
    mln_lang_array_t *array = (mln_lang_array_t *)udata;
    mln_lang_array_elem_t *elem = (mln_lang_array_elem_t *)rn_data;
    mln_lang_var_t *var;

    if ((var = mln_lang_array_getAndNew(array->ctx, array, elem->key)) == NULL) {
        return -1;
    }
    if (mln_lang_var_setValue(array->ctx, var, elem->value) < 0) {
        return -1;
    }
    return 0;
}
