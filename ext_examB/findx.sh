#!/bin/bash
find test_dir -name 'xfile' | xargs sed 's/char/int/g' > output

