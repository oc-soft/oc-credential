
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

function print_result()
{
  awk -v num=$1 -e 'BEGIN {
    no_leaks=0;
  }
  /^No memory leaks/ { no_leaks=1; }
  END {
    if (no_leaks) {
      msg1 = "ok"; 
    } else {
      msg1 = "not ok";
      msg2 = " - leak detected"
    }
    print msg1 " " num msg2
  }' 
}

test_data='https,example.com, ,user-1,password-1'

echo 1..2

mem_t_w_path=mem_trace_writing
mem_t_e_path=mem_trace_erasing


IFS=''
export MALLOC_TRACE=$mem_t_w_path
export LD_PRELOAD=libc_malloc_debug.so
echo $test_data | write_test_data 1
export MALLOC_TRACE=$mem_t_e_path
echo $test_data | erase_test_data 1
unset IFS


mtrace ./credential-oc/credhelper $mem_t_w_path | print_result 1
mtrace ./credential-oc/credhelper $mem_t_e_path | print_result 2

#! vi: se ts=2 sw=2 et:
