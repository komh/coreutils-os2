extproc sh

./configure --prefix=/usr \
            --disable-nls \
            --without-gmp \
            FORCE_UNSAFE_CONFIGURE=1 \
            "$@"
