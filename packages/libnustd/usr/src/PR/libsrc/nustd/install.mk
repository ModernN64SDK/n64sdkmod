HFILES = include/ctype.h include/malloc.h include/math.h include/mathdef.h \
	 include/setjmp.h include/stdio.h include/stdlib.h include/string.h

install: all
	$(V)mkdir -p /usr/lib/n64 /usr/include/n64/nustd
	@$(PRINT) "$(GREEN)Copying libnustd.a$(NO_COL)\n"
	$(V)cp $(BUILD_DIR_BASE)/libnustd/libnustd.a /usr/lib/n64/libnustd.a
	@$(PRINT) "$(GREEN)Copying libnustd_d.a$(NO_COL)\n"
	$(V)cp $(BUILD_DIR_BASE)/libnustd_d/libnustd_d.a /usr/lib/n64/libnustd_d.a
	@$(PRINT) "$(GREEN)Copying headers$(NO_COL)\n"
	$(V)$(foreach var,$(HFILES),cp $(var) /usr/include/n64/nustd/;)

# for internal apt use only
pkginstall: all
	$(V)mkdir -p /home/crash/n64dev/n64sdkmod/packages/libnustd/usr/lib/n64 /home/crash/n64dev/n64sdkmod/packages/libnustd/usr/include/n64/nustd
	@$(PRINT) "$(GREEN)Copying libnustd.a$(NO_COL)\n"
	$(V)cp $(BUILD_DIR_BASE)/libnustd/libnustd.a /home/crash/n64dev/n64sdkmod/packages/libnustd/usr/lib/n64/libnustd.a
	@$(PRINT) "$(GREEN)Copying libnustd_d.a$(NO_COL)\n"
	$(V)cp $(BUILD_DIR_BASE)/libnustd_d/libnustd_d.a /home/crash/n64dev/n64sdkmod/packages/libnustd/usr/lib/n64/libnustd_d.a
	@$(PRINT) "$(GREEN)Copying headers$(NO_COL)\n"
	$(V)$(foreach var,$(HFILES),cp $(var) /home/crash/n64dev/n64sdkmod/packages/libnustd/usr/include/n64/nustd/;)