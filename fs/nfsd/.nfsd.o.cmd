cmd_fs/nfsd/nfsd.o := mipsel-oe-linux-ld  -m elf32ltsmip   -r -o fs/nfsd/nfsd.o fs/nfsd/nfssvc.o fs/nfsd/nfsctl.o fs/nfsd/nfsproc.o fs/nfsd/nfsfh.o fs/nfsd/vfs.o fs/nfsd/export.o fs/nfsd/auth.o fs/nfsd/lockd.o fs/nfsd/nfscache.o fs/nfsd/nfsxdr.o fs/nfsd/stats.o 
