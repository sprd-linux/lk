SECTIONS {
    .fsymtab : ALIGN(16){
        KEEP (*(.fsymtab))
    }

    .fstrtab : ALIGN(16){
        KEEP (*(.fstrtab))
    }
}
INSERT BEFORE .bss;
