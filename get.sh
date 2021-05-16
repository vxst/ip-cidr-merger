curl -H "Accept: application/vnd.github.v3+json" https://api.github.com/meta | jq '.actions[]' | sed -e 's/"//g' 
