#!/usr/env zsh
# 指定時間ごとに監視を行うシェルスクリプト
#
# Author : Yuta Hayashibe
# Lisence  : Public Domain
# Arguments : process-name, mail-subject, mail-body
#
# 第1引数	バッチジョブに投げたシェルスクリプトの名前
# 第2引数	メール送信コマンドに与える第1引数(省略可)
# 第3引数	メール送信コマンドに与える第2引数(省略可)


PROCNAME=${1} #プロセス名の部分文字列を指定
INTERVAL=5      #監視間隔
MAIL=コマンド名        #成功メール送信コマンド

echo START
date
while true
do
        if [[ $(qstat |grep ${1} |wc -l) -eq  0 ]];
        then
            date && ${MAIL} ${2} ${3} && echo 'Program is dead!!' && exit
        fi

        sleep ${INTERVAL};
done

