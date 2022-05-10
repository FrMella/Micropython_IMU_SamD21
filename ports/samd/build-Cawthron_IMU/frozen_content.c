//
// Content for MICROPY_MODULE_FROZEN_STR
//
#include <stdint.h>
#define MP_FROZEN_STR_NAMES \

const uint32_t mp_frozen_str_sizes[] = { 0 };
const char mp_frozen_str_content[] = {
"\0"
};

//
// Content for MICROPY_MODULE_FROZEN_MPY
//
#include "py/mpconfig.h"
#include "py/objint.h"
#include "py/objstr.h"
#include "py/emitglue.h"
#include "py/nativeglue.h"

#if MICROPY_LONGINT_IMPL != 2
#error "incompatible MICROPY_LONGINT_IMPL"
#endif

#if MPZ_DIG_SIZE != 16
#error "incompatible MPZ_DIG_SIZE"
#endif

#if MICROPY_PY_BUILTINS_FLOAT
typedef struct _mp_obj_float_t {
    mp_obj_base_t base;
    mp_float_t value;
} mp_obj_float_t;
#endif

#if MICROPY_PY_BUILTINS_COMPLEX
typedef struct _mp_obj_complex_t {
    mp_obj_base_t base;
    mp_float_t real;
    mp_float_t imag;
} mp_obj_complex_t;
#endif

enum {
    MP_QSTR__boot_dot_py = MP_QSTRnumber_of,
    MP_QSTR_bdev,
    MP_QSTR_vfs,
};

const qstr_hash_t mp_qstr_frozen_const_hashes[] = {
    171,
    112,
    134,
};

const qstr_len_t mp_qstr_frozen_const_lengths[] = {
    8,
    4,
    3,
};

extern const qstr_pool_t mp_qstr_const_pool;
const qstr_pool_t mp_qstr_frozen_const_pool = {
    &mp_qstr_const_pool, // previous pool
    MP_QSTRnumber_of, // previous pool size
    3, // allocated entries
    3, // used entries
    (qstr_hash_t *)mp_qstr_frozen_const_hashes,
    (qstr_len_t *)mp_qstr_frozen_const_lengths,
    {
        "_boot.py",
        "bdev",
        "vfs",
    },
};

////////////////////////////////////////////////////////////////////////////////
// frozen module _boot
// - original source file: build-Cawthron_IMU/frozen_mpy/_boot.mpy
// - frozen file name: _boot.py
// - .mpy header: 4d:06:02:1f

// frozen bytecode for file _boot.py, scope _boot__lt_module_gt_
static const byte fun_data__boot__lt_module_gt_[114] = {
    0x2c,0x18, // prelude
    0x01, // names: <module>
    0x26,0x26,0x46,0x29,0x68,0x22,0x4d,0x2b,0x2d,0x4b,0x27, // code info
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x02, // IMPORT_NAME 'gc'
    0x16,0x02, // STORE_NAME 'gc'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x03, // IMPORT_NAME 'uos'
    0x16,0x03, // STORE_NAME 'uos'
    0x80, // LOAD_CONST_SMALL_INT 0
    0x51, // LOAD_CONST_NONE
    0x1b,0x04, // IMPORT_NAME 'samd'
    0x16,0x04, // STORE_NAME 'samd'
    0x11,0x04, // LOAD_NAME 'samd'
    0x13,0x05, // LOAD_ATTR 'Flash'
    0x14,0x06, // LOAD_METHOD 'flash_init'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x11,0x04, // LOAD_NAME 'samd'
    0x14,0x05, // LOAD_METHOD 'Flash'
    0x36,0x00, // CALL_METHOD 0
    0x16,0x07, // STORE_NAME 'bdev'
    0x48,0x0c, // SETUP_EXCEPT 12
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x08, // LOAD_METHOD 'VfsLfs1'
    0x11,0x07, // LOAD_NAME 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0x16,0x09, // STORE_NAME 'vfs'
    0x4a,0x19, // POP_EXCEPT_JUMP 25
    0x59, // POP_TOP
    0x11,0x03, // LOAD_NAME 'uos'
    0x13,0x08, // LOAD_ATTR 'VfsLfs1'
    0x14,0x0a, // LOAD_METHOD 'mkfs'
    0x11,0x07, // LOAD_NAME 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0x59, // POP_TOP
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x08, // LOAD_METHOD 'VfsLfs1'
    0x11,0x07, // LOAD_NAME 'bdev'
    0x36,0x01, // CALL_METHOD 1
    0x16,0x09, // STORE_NAME 'vfs'
    0x4a,0x01, // POP_EXCEPT_JUMP 1
    0x5d, // END_FINALLY
    0x11,0x03, // LOAD_NAME 'uos'
    0x14,0x0b, // LOAD_METHOD 'mount'
    0x11,0x09, // LOAD_NAME 'vfs'
    0x10,0x0c, // LOAD_CONST_STRING '/'
    0x36,0x02, // CALL_METHOD 2
    0x59, // POP_TOP
    0x11,0x02, // LOAD_NAME 'gc'
    0x14,0x0d, // LOAD_METHOD 'collect'
    0x36,0x00, // CALL_METHOD 0
    0x59, // POP_TOP
    0x19,0x03, // DELETE_NAME 'uos'
    0x19,0x09, // DELETE_NAME 'vfs'
    0x19,0x02, // DELETE_NAME 'gc'
    0x51, // LOAD_CONST_NONE
    0x63, // RETURN_VALUE
};
static const mp_raw_code_t raw_code__boot__lt_module_gt_ = {
    .kind = MP_CODE_BYTECODE,
    .scope_flags = 0x00,
    .n_pos_args = 0,
    .fun_data = fun_data__boot__lt_module_gt_,
    #if MICROPY_PERSISTENT_CODE_SAVE || MICROPY_DEBUG_PRINTERS
    .fun_data_len = 114,
    #endif
    .children = NULL,
    #if MICROPY_PERSISTENT_CODE_SAVE
    .n_children = 0,
    #if MICROPY_PY_SYS_SETTRACE
    .prelude = {
        .n_state = 6,
        .n_exc_stack = 1,
        .scope_flags = 0,
        .n_pos_args = 0,
        .n_kwonly_args = 0,
        .n_def_pos_args = 0,
        .qstr_block_name_idx = 1,
        .line_info = fun_data__boot__lt_module_gt_ + 3,
        .opcodes = fun_data__boot__lt_module_gt_ + 14,
    },
    .line_of_definition = 0,
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .prelude_offset = 0,
    .n_qstr = 0,
    .qstr_link = NULL,
    #endif
    #endif
    #if MICROPY_EMIT_MACHINE_CODE
    .type_sig = 0,
    #endif
};

static const qstr_short_t const_qstr_table_data__boot[14] = {
    MP_QSTR__boot_dot_py,
    MP_QSTR__lt_module_gt_,
    MP_QSTR_gc,
    MP_QSTR_uos,
    MP_QSTR_samd,
    MP_QSTR_Flash,
    MP_QSTR_flash_init,
    MP_QSTR_bdev,
    MP_QSTR_VfsLfs1,
    MP_QSTR_vfs,
    MP_QSTR_mkfs,
    MP_QSTR_mount,
    MP_QSTR__slash_,
    MP_QSTR_collect,
};

static const mp_frozen_module_t frozen_module__boot = {
    .constants = {
        .qstr_table = (qstr_short_t *)&const_qstr_table_data__boot,
        .obj_table = NULL,
    },
    .rc = &raw_code__boot__lt_module_gt_,
};

////////////////////////////////////////////////////////////////////////////////
// collection of all frozen modules

const char mp_frozen_names[] = {
    #ifdef MP_FROZEN_STR_NAMES
    MP_FROZEN_STR_NAMES
    #endif
    "_boot.py\0"
    "\0"
};

const mp_frozen_module_t *const mp_frozen_mpy_content[] = {
    &frozen_module__boot,
};

#ifdef MICROPY_FROZEN_LIST_ITEM
MICROPY_FROZEN_LIST_ITEM("_boot", "_boot.py")
#endif

/*
byte sizes:
qstr content: 3 unique, 24 bytes
bc content: 114
const str content: 0
const int content: 0
const obj content: 0
const table qstr content: 0 entries, 0 bytes
const table ptr content: 0 entries, 0 bytes
raw code content: 1 * 4 = 16
mp_frozen_mpy_names_content: 10
mp_frozen_mpy_content_size: 4
total: 168
*/
