function quoted(str) {return (str != null) ? '"' + str + '"' : '""';}try{ (new_browser == undefined); } catch(err) { var new_browser = 'false'; }try{(defined_title == undefined);}catch(err){var defined_title = '';}try{(enc == undefined);}catch(err){var enc = 'UTF-8';}if(site_id == 40 || site_id == 10){window.process_url = 'http://www-128.ibm.com/developerworks/everywhere/ew?lo=' + escape(document.location) + '&amp;fe=' + escape(dw_rss_feed) + '&amp;nu=' + escape(num_of_articles) + '&amp;sid=' + escape(site_id) + '&amp;enc=' + escape(enc) + '&amp;new_browser=' + new_browser + '&amp;defined_title=' + escape(defined_title);}else{window.process_url = 'http://www-128.ibm.com/developerworks/everywhere/ew?lo=' + escape(document.location) + '&amp;fe=' + escape(dw_rss_feed) + '&amp;nu=' + escape(num_of_articles) + '&amp;sid=' + escape(site_id) + '&amp;new_browser=' + new_browser + '&amp;defined_title=' + escape(defined_title);}document.write('<link href="//www-128.ibm.com/developerworks/everywhere/css/gizmos.css" type="text/css" rel="stylesheet"/>');
document.write('<script type="text/javascript" src=' + quoted(window.process_url) + '></script>');
document.write('<script type="text/javascript" src="http://www-128.ibm.com/developerworks/everywhere/js/prototype.js"></script>');
document.write('<script type="text/javascript" src=http://www-128.ibm.com/developerworks/everywhere/js/feed.js></script>');
