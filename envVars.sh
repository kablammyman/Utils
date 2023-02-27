#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "MY_STRING_UTILS="$DIR"/StringUtils">>/etc/environment
echo "MY_LOG_UTILS="$DIR"/LogUtils">>/etc/environment
echo "MY_HASH_UTILS="$DIR"/HashUtils">>/etc/environment
echo "MY_CFG_UTILS="$DIR"/CFGUtils">>/etc/environment
echo "MY_FILE_UTILS="$DIR"/FileUtils">>/etc/environment
echo "MY_COMMANDLINE_UTILS="$DIR"/CommandLineUtils">>/etc/environment
echo "MY_NET_UTILS="$DIR"/NetUtils">>/etc/environment
echo "MY_SQLITE3_UTILS="$DIR"/SQLiteUtils">>/etc/environment
echo "MY_GAME_UTILS="$DIR"/GameUtils">>/etc/environment
echo "MY_TAGGING_UTILS="$DIR"/TaggingUtils">>/etc/environment
echo "MY_CURL_UTILS="$DIR"/CurlUtils">>/etc/environment
echo "MY_X86_DEBUG_LIBS="$DIR"/Debug">>/etc/environment
echo "MY_X64_DEBUG_LIBS="$DIR"/x64/Debug">>/etc/environment

echo "MY_X86_RELEASE_LIBS="$DIR"/Release">>/etc/environment
echo "MY_X64_RELEASE_LIBS="$DIR"/x64/Release">>/etc/environment


#source ~/.bashrc
#source ~/.profile
#echo "do you wanna restart your computer to apply changes in /etc/environment file? yes(y)no(n)"
#read restart
#case $restart in
#    y) sudo shutdown -r 0;;
#    n) echo "don't forget to restart your computer manually";;
#esac
exit
