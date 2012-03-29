<?php

/*
 *  Display SHA512 encoded string for inputs.
 *
 */

if( isset($_POST["lines"])){
	$list = explode("\r\n", $_POST["lines"] );
	$sha4html = "";

	foreach($list as $line){
		$sha = hash(sha512, $line);
	$original .= <<<__EOT__
${line}<br />
__EOT__;
	$sha4html .= <<<__EOT__
${sha}<br />
__EOT__;
	}
};

$output4html = <<<__EOT__
<html> 
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=EUC-JP">
<title>Get SHA512</title> 
</head>
<body>
		<form action="${_SERVER['SCRIPT_NAME']}" method="post" accept-charset="utf-8"> 
		<textarea name="lines" rows="4" cols="40"></textarea>
		<br />
		<input type="submit" value="GET SHA512" />
		</form>
</body>
</html>

	<h1>Original</h1>	
	${original}

	<br />
	<br />

	<h1>SHA512</h1>
	${sha4html}

__EOT__;


print ${output4html};

