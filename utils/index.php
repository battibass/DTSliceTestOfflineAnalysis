<html>
<head>
<title><?php echo getcwd(); ?></title>
<style type='text/css'>
body {
    font-family: "Candara", sans-serif;
    font-size: 9pt;
    line-height: 10.5pt;
}

h1 {
  text-align:center;
  padding-bottom: 5px;
  border-bottom: 2px solid lightblue;
}

h2 {
  border: 2px solid gray;
  padding-top: 5px;
  padding-bottom: 5px;
  padding-left: 5px;
  color: blue;
}

ul.subdirs {
    padding: 0pt;
}

li.subdirs {
    display: inline;
    list-style-type: none;
    padding-right: 20px;
}

div.box {
  border: 2px dotted lightgray;
  padding-top: 5px;
  padding-bottom: 5px;
  padding-left: 5px;
  padding-right: 5px;
}

.pic-container {
  border-bottom: 2px dotted lightblue;
  display: grid;
  grid-template-columns: 1fr 1fr 1fr 1fr;
  grid-gap: 10px;
  padding: 10px;
}

.pic h3 { 
    font-size: 11pt;
    margin: 0.5em 1em 0.2em 1em;
}

.pic p {
    font-size: 11pt;
    margin: 0.2em 1em 0.1em 1em;
}

.pic {
    float: left;
    background-color: white;
    border: 1px solid #ccc;
    padding: 2px;
    text-align: center;
    margin: 2px 10px 10px 2px;
    /* -moz-box-shadow: 7px 5px 5px rgb(80,80,80);    /* Firefox 3.5 */
    /* -webkit-box-shadow: 7px 5px 5px rgb(80,80,80); /* Chrome, Safari */
    /* box-shadow: 7px 5px 5px rgb(80,80,80);         /* New browsers */
}
a { text-decoration: none; color: rgb(0,0,140); }
a:hover { text-decoration: underline; color: rgb(50,50,255); }
div.dirlinks h2 {  margin-bottom: 4pt; margin-left: -24pt; color: rgb(0,0,140);  }
div.dirlinks {  margin: 0 24pt; } 
div.dirlinks a {
    font-size: 11pt; font-weight: bold;
    padding: 0 0.5em; 
}
</style>
<script>
function ClearFields() {

     document.getElementById("match").value = "";
}
</script>
</head>
<body>
<h1><?php echo substr(dirname(__FILE__), strpos(dirname(__FILE__), 'SliceTest/')); ?></h1>
<?php
$has_subs = false;
foreach (glob("*") as $filename) {
    if (is_dir($filename) && !preg_match("/^\..*|.*private.*/", $filename)) {
        $has_subs = true;
        break;
    }
}
#if ($has_subs) {
    print "<div class=\"dirlinks\">\n";
    print "<h2>Directories</h2>\n";
    print "<a href=\"../\">[parent]</a> ";
    foreach (glob("*") as $filename) {
        if (is_dir($filename) && (!preg_match("/^\..*|.*private.*/", $filename))) {
            print " <a href=\"$filename\">[$filename]</a>";
        }
    }
    print "</div>";
#}

foreach (array("00_README.txt", "README.txt", "readme.txt") as $readme) {
    if (file_exists($readme)) {
        print "<pre class='readme'>\n"; readfile($readme); print "</pre>";
    }
}
?>

<h2><a name="plots">Plots</a></h2>
<div class="box">
<p><form>Filter: <input type="text" name="match" id="match" size="30" value="<?php if (isset($_GET['match'])) print htmlspecialchars($_GET['match']);  ?>" /><input type="Submit" value="Go" /><button type="button" onclick="ClearFields();">Clear</button></form></p>
</div>
<div>
<?php

# Configure the plot size (should also play above with CSS)
$plotwidth = '300px';

# Say the name of the object in the root files you want to display by default
$roothistoname = 'canvas';

# Path to the JSRoot installation
$jsrootpath='https://dt-sx5.web.cern.ch/dt-sx5/Results/SliceTest/jsroot/';
$thisfolder=str_replace("/SliceTest/","../",substr(dirname(__FILE__),strpos(dirname(__FILE__), '/SliceTest/')));

# Extension that will be searched for files
$filenames = glob("*.png"); 

# Other possible formats for the files that will be considered
$other_exts = array('.pdf', '.cxx', '.eps', '.root', '.txt', '.C', '.gif');

# Start playing...
$displayed = array();
array_push($displayed,basename($_SERVER['PHP_SELF']));
sort($filenames);

$canvas_groups = array();

foreach ($filenames as $filename) { 
    $group = substr($filename, 0, 3);
    array_push($canvas_groups, $group);
}

$canvas_groups = array_unique($canvas_groups);

foreach ($canvas_groups as $group) {
    print "<div class=\"pic-container\">\n";
    foreach ($filenames as $filename) {
        if (substr($filename, 0, 3) ==  $group) {
            if (isset($_GET['match']) && !fnmatch('*'.$_GET['match'].'*', $filename)) continue;
            array_push($displayed, $filename);
            print "<div class='pic'>\n";
            $title = str_replace('.png', '', $filename);
            print "<h3><a href=\"$filename\">$title</a></h3>";
            print "<a href=\"$filename\"><img src=\"$filename\" style=\"border: none; width: $plotwidth; \"></a>";
            $others = array();
            foreach ($other_exts as $ex) {
                $other_filename = str_replace('.png', $ex, $filename);
                if (file_exists($other_filename)) {
	                if ($ex == '.root') {
                        array_push($others, "<a class=\"file\" href=\"$jsrootpath?file=$thisfolder/$other_filename&item=$roothistoname \">[<img src=\"https://root.cern/favicon.ico\">" . $ex . "]</a>");
	                } else {
                        array_push($others, "<a class=\"file\" href=\"$other_filename\">[" . $ex . "]</a>");
	                }
                    if ($ex != '.txt') array_push($displayed, $other_filename);
                }
            }
            if ($others) print "<p>Also as ".implode(', ',$others)."</p>\n";
            else print "<p>&nbsp;</p>\n";
            print "</div>\n";
        }
    }
    print "</div>\n";
}

?>
</div>
<div style="display: block; clear:both;">
<h2><a name="files">Other files</a></h2>
<ul>
<?
foreach (glob("*") as $filename) {
    if ($_GET['noplots'] || !in_array($filename, $displayed)) {
        if (isset($_GET['match']) && !fnmatch('*'.$_GET['match'].'*', $filename)) continue;
        if ($filename == 'index.php') continue;
        if (is_dir($filename)) {
            print "<li>[DIR] <a href=\"$filename\">$filename</a></li>";
        } elseif (strpos($filename, ".root")) {
            print "<li><a href=\"$filename\">$filename</a> <a class=\"file\" href=\"$jsrootpath?file=$thisfolder/$filename \">[<img src=\"https://root.cern/favicon.ico\"> ROOT]</a></li>";
        } else {
            print "<li><a href=\"$filename\">$filename</a></li>";
        }
    }
}
?>
</ul>
</div>
</body>
</html>
