extproc sh

./configure --disable-nls \
            --without-gmp \
            FORCE_UNSAFE_CONFIGURE=1 \
            "$@"
