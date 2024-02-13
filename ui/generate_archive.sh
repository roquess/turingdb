#!/bin/bash

set -e

if [ ! -d "$1" ] ; then
    echo 'ERROR: ./generate_archive.sh /path/to/site /path/to/output_dir'
    exit 1
fi

if [ ! -d  "$2" ] ; then
    echo 'ERROR: ./generate_archive.sh /path/to/site /path/to/output_dir'
    exit 1
fi

site_dir=$(readlink -e $1)
out_dir=$(readlink -e $2)
prototype_site_dir=$(readlink -e $3)
prototype_out_dir=$(readlink -e $4)

archive_path=$out_dir/site.tar
prototype_archive_path=$prototype_out_dir/sitePrototype.tar

cd $site_dir && ./build.sh
tar -zcvf $archive_path site

cd $prototype_site_dir && ./build.sh
tar -zcvf $prototype_archive_path site

echo "
#pragma once

#define INCBIN_PREFIX globfs_
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include \"incbin.h\"

INCBIN(site, \"./site.tar\");
INCBIN(sitePrototype, \"./sitePrototype.tar\");
" > $out_dir/SiteArchiveData.h
