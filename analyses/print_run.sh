if [ -z "$CERN_USER" ]; then
    export CERN_USER=$(whoami)
fi

echo "Using $CERN_USER as CERN_USER"
mkdir /eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/run$1/
echo "INFO: Deleting old files"
cp results_run$1.root /eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/run$1/
echo "INFO: Ntuple copied"
cp /eos/home-j/jleonhol/backup/index_withFolders_php /eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/run$1/index.php 
for i in ./run$1/*
do
  if [ -d "$i" ]
  then
    echo "INFO: Copying files in " $i
    if [ ! -d "/eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/$i" ]
    then
        mkdir /eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/$i
    fi
    cp $i/* /eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/$i
    cp /eos/home-j/jleonhol/backup/index_noFolders_php /eos/home-${CERN_USER:0:1}/${CERN_USER}/www/SliceTest/$i/index.php 
  fi
done
rm -rf ./run$1

