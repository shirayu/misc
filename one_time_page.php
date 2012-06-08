<?php
/* 
 * 1度だけ見れるサイト
*/
?>



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title></title>
</head>

<body>

<?php
	if ( file_exists( "lock" )) {
		print "有効期限切れのため，表示できません.";
	} else {

print<<<EOF
一度だけ表示する内容
EOF;

		if(!touch('lock', time() ))
		{
		    echo 'ロックファイル作成エラー';
		}
		else
		{
			//ok
		}
	}


?>



</body>
</html>

