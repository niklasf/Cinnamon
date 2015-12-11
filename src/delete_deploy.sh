~/workspace/assembla_git/chess-utility/script/remote/bash_async.sh "/home/geko/Kill.sh"
~/workspace/assembla_git/chess-utility/script/remote/bash_async.sh "rm -fr test"

array=( $IPS )

make clean;make cinnamon64-modern-INTEL

rm -fr /tmp/deploy/
UUID=$(cat /proc/sys/kernel/random/uuid)
mkdir -p /tmp/deploy/$UUID
cp -r ../src /tmp/deploy/$UUID
cd /tmp/deploy
rm -fr $UUID/src/*.o $UUID/src/gtb/Linux $UUID/src/gtb/Windows $UUID/src/gtb/OSX
tar -czf $UUID.tar.gz $UUID

for ip in "${array[@]}"
do	
	ssh geko@$ip "mkdir test" >/dev/null
	echo "scp /tmp/deploy/$UUID.tar.gz geko@$ip:test"
	scp /tmp/deploy/$UUID.tar.gz geko@$ip:test
done
cd ~/workspace/assembla_git/chess-utility/script/remote
./cute.sh
date
exit 0
 


