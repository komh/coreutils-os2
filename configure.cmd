extproc sh

n=configure
test -f "./$n." || { echo "\`./$n' not found !!!"; exit 1; }

opts="--disable-nls --without-gmp FORCE_UNSAFE_CONFIGURE=1"
"./$n" $opts "$@" 2>&1 | tee "$n.log"
