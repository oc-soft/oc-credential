echo 1..1  

echo 'username=baerer
protocol=https
host=example.org
password=pass
' | ./credential-ocs/credhelper store

state=$?
if [[ $state == 0 ]]; then
  echo 'ok 1 # succeeded credential store'
else
  exit $state
fi
# vi: se ts=2 sw=2 et:
