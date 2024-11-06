
# . ./setup_variables.sh
echo make -j$(nproc)
(cd ../build/tests && make all -j$(nproc))