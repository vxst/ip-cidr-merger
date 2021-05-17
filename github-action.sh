c++ -O2 -Wno-unused-result merge.cpp -o merge
curl -H "Accept: application/vnd.github.v3+json" https://api.github.com/meta | jq '.actions[]' | sed -e 's/"//g' | sed -e 's/^.*:.*$//g' > full_list
./merge < full_list > simple_list
