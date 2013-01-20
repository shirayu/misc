#! /bin/sh

DIRECTRY="/data/google-ngram/en/5gms"
OUT="./sub/"
mkdir -p ${OUT}
ORDER=5
LOG="./log"
PARALELL=20

find $DIRECTRY -type f |grep gz |xargs -l basename | cut -b5-8 | xargs -t -P ${PARALELL} -l -i build-sublm.pl --size ${ORDER} --ngrams "/bin/gunzip -c ${DIRECTRY}/5gm-{}.gz" --sublm "${OUT}LM.{}" --prune-singletons --witten-bell >> ${LOG} 2>&1

OUTFILE="./lm.gz"
echo "Merging language models into $outfile"
merge-sublm.pl --size ${ORDER} --sublm ${OUT}LM -lm ${OUTFILE}  >> ${LOG} 2>&1

echo You can remove ${OUT}
#rm -r ${OUT}

#exit
