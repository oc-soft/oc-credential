
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
  print_desc $1 $2 $3 $4 $5 | ./lmdcli store
}

function erase_data()
{
  print_desc $1 $2 $3 $4 $5 | ./lmdcli erase
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

test_data='https,example.com, ,user-1,password-1'

echo '1..1'

mem_t_w_path=mem_trace_writing
mem_t_e_path=mem_trace_erasing


IFS=''
export MALLOC_TRACE=$mem_t_w_path
export LD_PRELOAD=libc_malloc_debug.so
echo $test_data | write_test_data 1
export MALLOC_TRACE=$mem_t_e_path
echo $test_data | erase_test_data 1

mtrace ./lmdcli $mem_t_w_path
mtrace ./lmdcil $mem_t_e_path
unset IFS
echo 'ok 1'

#! vi: se ts=2 sw=2 et:
