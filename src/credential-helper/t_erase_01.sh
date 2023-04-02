
function print_desc()
{
  echo "protocol=$1
host=$2"
  local path
  local username
  local password
  path=`echo $3 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//'`
  username=`echo $4 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//'`
  password=`echo $5 | sed -e 's/^[[:space:]]*//' | sed -e 's/[[:space:]]*$//'`
  if [ -n $path ]; then
    echo "path=$path"
  fi
  if [ -n $username ]; then
    echo "username=$username"
  fi
  if [ -n $password ]; then
    echo "password=$password"
  fi
echo ""
}

function store_data()
{
  print_desc $1 $2 $3 $4 $5 | ./credential-ocs/credhelper store
}

function erase_data()
{
  print_desc $1 $2 $3 $4 $5 | ./credential-ocs/credhelper erase
}


function write_test_data()
{
  local idx
  local saved_ifs
  idx=1
  saved_ifs=$IFS
  while read -r line;
  do
    if (( $idx == $1 )); then
      IFS=',' 
      store_data $line  
      IFS=$saved_ifs
      break
    fi 
    let idx++
  done  
  IFS=$saved_ifs
}

function erase_test_data()
{
  local idx
  local saved_ifs
  local res
  idx=1
  res=0
  saved_ifs=$IFS
  while read -r line;
  do
    if (( $idx == $1 )); then
      IFS=',' 
      erase_data $line  
      res=$?
      IFS=$saved_ifs
      break
    fi 
    let idx++
  done  
  IFS=$saved_ifs
  return $res
}

test_data='https,example.com, ,user-1,password-1
https,example.com,subpath-2,user-1,password-2
https,example.com,subpath-3,user-1,password-3
https,example.com,subpath-4,user-1,password-4
https,example.com,subpath-5,user-1,password-5
https,example.com,subpath-6,user-1,password-6
https,example.com,subpath-7,user-1,password-7
https,example.com,subpath-8,user-1,password-8
https,example.com,subpath-9,user-1,password-9
https,example.com,subpath-10,user-1,password-10
https,example.com,subpath-1,user-1,password-11
https,example.com,subpath-2,user-1,password-12'

echo '1..1'
IFS=''

for (( idx = 1; idx <= 12; idx++ )); do
  echo $test_data | write_test_data $idx
done

for (( idx = 1; idx <= 12; idx++ )); do
  echo $test_data | erase_test_data $idx
  res=$?
  if (( $res != 0 )); then
    exit $res
  fi
done
unset IFS
echo 'ok 1'

#! vi: se ts=2 sw=2 et:
