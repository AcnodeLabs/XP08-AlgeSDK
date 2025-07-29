on Mac M1:
copy cnats (cmake) folder to mac

brew install openssl
export OPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3 #obtaned from 'brew --prefix openssl' 
cmake .. -DNATS_BUILD_STREAMING=OFF