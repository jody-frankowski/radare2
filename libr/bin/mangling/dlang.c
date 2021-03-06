/* radare - LGPL - Copyright 2015 - pancake */

#include <r_bin.h>

// The dlang-demangler is written in D and available at radare2-extras

static int is_dlang_symbol (const char *name) {
	if (!strncmp (name, "_D2", 2)) 
		return 1;
	if (!strncmp (name, "_D4", 2))
		return 1;
	return 0;
}

R_API int r_bin_lang_dlang(RBinFile *binfile) {
	RBinObject *o = binfile ? binfile->o : NULL;
	RBinInfo *info = o ? o->info : NULL;
	RBinSymbol *sym;
	RListIter *iter;
	int hasdlang = R_FALSE;
	const char *lib;

	if (!info)
		return R_FALSE;
	r_list_foreach (o->libs, iter, lib) {
		if (strstr (lib, "phobos")) {
			hasdlang = R_TRUE;
			break;
		}
	}
	if (!hasdlang) {
		r_list_foreach (o->symbols, iter, sym) {
			if (is_dlang_symbol (sym->name)) {
				hasdlang = R_TRUE;
				break;
			}
		}
	}
	if (hasdlang)
		info->lang = "dlang";
	return hasdlang;
}
