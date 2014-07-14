
#include "MIMEType.hpp"

std::map<std::string, std::string> MIMEType::exts = std::map<std::string, std::string>();
std::string MIMEType::default_type = "application/octet-stream";
bool MIMEType::initialized = false;

void MIMEType::init(){
  if(!initialized){
    initialized = true;

    set("3dm", "x-world/x-3dmf");
    set("3dmf", "x-world/x-3dmf");
    set("a", "application/octet-stream");
    set("aab", "application/x-authorware-bin");
    set("aam", "application/x-authorware-map");
    set("aas", "application/x-authorware-seg");
    set("abc", "text/vnd.abc");
    set("acgi", "text/html");
    set("afl", "video/animaflex");
    set("ai", "application/postscript");
    set("aif", "audio/aiff");
    set("aifc", "audio/aiff");
    set("aiff", "audio/aiff");
    set("aim", "application/x-aim");
    set("aip", "text/x-audiosoft-intra");
    set("ani", "application/x-navi-animation");
    set("aos", "application/x-nokia-9000-communicator-add-on-software");
    set("aps", "application/mime");
    set("arc", "application/octet-stream");
    set("arj", "application/arj");
    set("art", "image/x-jg");
    set("asf", "video/x-ms-asf");
    set("asm", "text/x-asm");
    set("asp", "text/asp");
    set("asx", "video/x-ms-asf");
    set("au", "audio/x-au");
    set("avi", "video/avi");
    set("avs", "video/avs-video");
    set("bcpio", "application/x-bcpio");
    set("bin", "application/octet-stream");
    set("bm", "image/bmp");
    set("bmp", "image/bmp");
    set("boo", "application/book");
    set("book", "application/book");
    set("boz", "application/x-bzip2");
    set("bsh", "application/x-bsh");
    set("bz", "application/x-bzip");
    set("bz2", "application/x-bzip2");
    set("c", "text/x-c");
    set("c++", "text/plain");
    set("cat", "application/vnd.ms-pki.seccat");
    set("cc", "text/x-c");
    set("ccad", "application/clariscad");
    set("cco", "application/x-cocoa");
    set("cdf", "application/cdf");
    set("cer", "application/pkix-cert");
    set("cha", "application/x-chat");
    set("chat", "application/x-chat");
    set("class", "application/java");
    set("com", "text/plain");
    set("conf", "text/plain");
    set("cpio", "application/x-cpio");
    set("cpp", "text/x-c");
    set("cpt", "application/mac-compactpro");
    set("crl", "application/pkcs-crl");
    set("crt", "application/pkix-cert");
    set("csh", "application/x-csh");
    set("css", "text/css");
    set("cxx", "text/plain");
    set("dcr", "application/x-director");
    set("deepv", "application/x-deepv");
    set("def", "text/plain");
    set("der", "application/x-x509-ca-cert");
    set("dif", "video/x-dv");
    set("dir", "application/x-director");
    set("dl", "video/dl");
    set("doc", "application/msword");
    set("dot", "application/msword");
    set("dp", "application/commonground");
    set("drw", "application/drafting");
    set("dump", "application/octet-stream");
    set("dv", "video/x-dv");
    set("dvi", "application/x-dvi");
    set("dwf", "model/vnd.dwf");
    set("dwg", "application/acad");
    set("dxf", "application/dxf");
    set("dxr", "application/x-director");
    set("el", "text/x-script.elisp");
    set("elc", "application/x-elc");
    set("env", "application/x-envoy");
    set("eps", "application/postscript");
    set("es", "application/x-esrehber");
    set("etx", "text/x-setext");
    set("evy", "application/envoy");
    set("exe", "application/octet-stream");
    set("f", "text/x-fortran");
    set("f77", "text/x-fortran");
    set("f90", "text/x-fortran");
    set("fdf", "application/vnd.fdf");
    set("fif", "image/fif");
    set("fli", "video/fli");
    set("flo", "image/florian");
    set("flx", "text/vnd.fmi.flexstor");
    set("fmf", "video/x-atomic3d-feature");
    set("for", "text/x-fortran");
    set("fpx", "image/vnd.fpx");
    set("frl", "application/freeloader");
    set("funk", "audio/make");
    set("g", "text/plain");
    set("g3", "image/g3fax");
    set("gif", "image/gif");
    set("gl", "video/gl");
    set("gsd", "audio/x-gsm");
    set("gsm", "audio/x-gsm");
    set("gsp", "application/x-gsp");
    set("gss", "application/x-gss");
    set("gtar", "application/x-gtar");
    set("gz", "application/x-gzip");
    set("gzip", "application/x-gzip");
    set("h", "text/x-h");
    set("hdf", "application/x-hdf");
    set("help", "application/x-helpfile");
    set("hgl", "application/vnd.hp-hpgl");
    set("hh", "text/x-h");
    set("hlp", "application/hlp");
    set("hpg", "application/vnd.hp-hpgl");
    set("hpgl", "application/vnd.hp-hpgl");
    set("hqx", "application/binhex");
    set("hta", "application/hta");
    set("htc", "text/x-component");
    set("htm", "text/html");
    set("html", "text/html");
    set("htmls", "text/html");
    set("htt", "text/webviewhtml");
    set("htx", "text/html");
    set("ice", "x-conference/x-cooltalk");
    set("ico", "image/x-icon");
    set("idc", "text/plain");
    set("ief", "image/ief");
    set("iefs", "image/ief");
    set("iges", "application/iges");
    set("igs", "application/iges");
    set("ima", "application/x-ima");
    set("imap", "application/x-httpd-imap");
    set("inf", "application/inf");
    set("ins", "application/x-internett-signup");
    set("ip", "application/x-ip2");
    set("isu", "video/x-isvideo");
    set("it", "audio/it");
    set("iv", "application/x-inventor");
    set("ivr", "i-world/i-vrml");
    set("ivy", "application/x-livescreen");
    set("jam", "audio/x-jam");
    set("jav", "text/x-java-source");
    set("java", "text/x-java-source");
    set("jcm", "application/x-java-commerce");
    set("jfif", "image/jpeg");
    set("jfif", "image/pjpeg");
    set("jfif-tbnl", "image/jpeg");
    set("jpe", "image/jpeg");
    set("jpeg", "image/jpeg");
    set("jpg", "image/jpeg");
    set("js", "text/javascript");
    set("jut", "image/jutvision");
    set("kar", "music/x-karaoke");
    set("ksh", "application/x-ksh");
    set("la", "audio/nspaudio");
    set("lam", "audio/x-liveaudio");
    set("latex", "application/x-latex");
    set("lha", "application/lha");
    set("lhx", "application/octet-stream");
    set("list", "text/plain");
    set("lma", "audio/nspaudio");
    set("log", "text/plain");
    set("lsp", "application/x-lisp");
    set("lst", "text/plain");
    set("lsx", "text/x-la-asf");
    set("ltx", "application/x-latex");
    set("lzh", "application/x-lzh");
    set("lzx", "application/lzx");
    set("m", "text/x-m");
    set("m1v", "video/mpeg");
    set("m2a", "audio/mpeg");
    set("m2v", "video/mpeg");
    set("m3u", "audio/x-mpequrl");
    set("man", "application/x-troff-man");
    set("map", "application/x-navimap");
    set("mar", "text/plain");
    set("mbd", "application/mbedlet");
    set("mc$", "application/x-magic-cap-package-1.0");
    set("mcd", "application/mcad");
    set("mcf", "image/vasa");
    set("mcp", "application/netmc");
    set("me", "application/x-troff-me");
    set("mht", "message/rfc822");
    set("mhtml", "message/rfc822");
    set("mid", "application/x-midi");
    set("mid", "audio/midi");
    set("midi", "audio/midi");
    set("mif", "application/x-mif");
    set("mime", "www/mime");
    set("mjf", "audio/x-vnd.audioexplosion.mjuicemediafile");
    set("mjpg", "video/x-motion-jpeg");
    set("mm", "application/x-meme");
    set("mme", "application/base64");
    set("mod", "audio/mod");
    set("moov", "video/quicktime");
    set("mov", "video/quicktime");
    set("movie", "video/x-sgi-movie");
    set("mp2", "audio/mpeg");
    set("mp3", "audio/mpeg3");
    set("mpa", "video/mpeg");
    set("mpc", "application/x-project");
    set("mpe", "video/mpeg");
    set("mpeg", "video/mpeg");
    set("mpg", "video/mpeg");
    set("mpga", "audio/mpeg");
    set("mpp", "application/vnd.ms-project");
    set("mpt", "application/x-project");
    set("mpv", "application/x-project");
    set("mpx", "application/x-project");
    set("mrc", "application/marc");
    set("ms", "application/x-troff-ms");
    set("mv", "video/x-sgi-movie");
    set("my", "audio/make");
    set("mzz", "application/x-vnd.audioexplosion.mzz");
    set("nap", "image/naplps");
    set("naplps", "image/naplps");
    set("nc", "application/x-netcdf");
    set("ncm", "application/vnd.nokia.configuration-message");
    set("nif", "image/x-niff");
    set("niff", "image/x-niff");
    set("nix", "application/x-mix-transfer");
    set("nsc", "application/x-conference");
    set("nvd", "application/x-navidoc");
    set("o", "application/octet-stream");
    set("oda", "application/oda");
    set("omc", "application/x-omc");
    set("omcd", "application/x-omcdatamaker");
    set("omcr", "application/x-omcregerator");
    set("p", "text/x-pascal");
    set("p10", "application/pkcs10");
    set("p12", "application/pkcs-12");
    set("p7a", "application/x-pkcs7-signature");
    set("p7c", "application/pkcs7-mime");
    set("p7m", "application/pkcs7-mime");
    set("p7r", "application/x-pkcs7-certreqresp");
    set("p7s", "application/pkcs7-signature");
    set("part", "application/pro_eng");
    set("pas", "text/pascal");
    set("pbm", "image/x-portable-bitmap");
    set("pcl", "application/vnd.hp-pcl");
    set("pct", "image/x-pict");
    set("pcx", "image/x-pcx");
    set("pdb", "chemical/x-pdb");
    set("pdf", "application/pdf");
    set("pfunk", "audio/make");
    set("pgm", "image/x-portable-graymap");
    set("pic", "image/pict");
    set("pict", "image/pict");
    set("pkg", "application/x-newton-compatible-pkg");
    set("pko", "application/vnd.ms-pki.pko");
    set("pl", "text/x-script.perl");
    set("plx", "application/x-pixclscript");
    set("pm", "image/x-xpixmap");
    set("pm4", "application/x-pagemaker");
    set("pm5", "application/x-pagemaker");
    set("png", "image/png");
    set("pnm", "image/x-portable-anymap");
    set("pot", "application/mspowerpoint");
    set("pov", "model/x-pov");
    set("ppa", "application/vnd.ms-powerpoint");
    set("ppm", "image/x-portable-pixmap");
    set("pps", "application/mspowerpoint");
    set("ppt", "application/mspowerpoint");
    set("ppz", "application/mspowerpoint");
    set("pre", "application/x-freelance");
    set("prt", "application/pro_eng");
    set("ps", "application/postscript");
    set("psd", "application/octet-stream");
    set("pvu", "paleovu/x-pv");
    set("pwz", "application/vnd.ms-powerpoint");
    set("py", "text/x-script.phyton");
    set("pyc", "applicaiton/x-bytecode.python");
    set("qcp", "audio/vnd.qcelp");
    set("qd3", "x-world/x-3dmf");
    set("qd3d", "x-world/x-3dmf");
    set("qif", "image/x-quicktime");
    set("qt", "video/quicktime");
    set("qtc", "video/x-qtc");
    set("qti", "image/x-quicktime");
    set("qtif", "image/x-quicktime");
    set("ra", "audio/x-realaudio");
    set("ram", "audio/x-pn-realaudio");
    set("ras", "image/x-cmu-raster");
    set("rast", "image/cmu-raster");
    set("rexx", "text/x-script.rexx");
    set("rf", "image/vnd.rn-realflash");
    set("rgb", "image/x-rgb");
    set("rm", "application/vnd.rn-realmedia");
    set("rmi", "audio/mid");
    set("rmm", "audio/x-pn-realaudio");
    set("rmp", "audio/x-pn-realaudio");
    set("rng", "application/ringing-tones");
    set("rnx", "application/vnd.rn-realplayer");
    set("roff", "application/x-troff");
    set("rp", "image/vnd.rn-realpix");
    set("rpm", "audio/x-pn-realaudio-plugin");
    set("rt", "text/richtext");
    set("rtf", "application/rtf");
    set("rtx", "application/rtf");
    set("rv", "video/vnd.rn-realvideo");
    set("s", "text/x-asm");
    set("s3m", "audio/s3m");
    set("saveme", "application/octet-stream");
    set("sbk", "application/x-tbook");
    set("scm", "application/x-lotusscreencam");
    set("sdml", "text/plain");
    set("sdp", "application/sdp");
    set("sdr", "application/sounder");
    set("sea", "application/sea");
    set("set", "application/set");
    set("sgm", "text/sgml");
    set("sgml", "text/sgml");
    set("sh", "text/x-script.sh");
    set("shar", "application/x-shar");
    set("shtml", "text/x-server-parsed-html");
    set("sid", "audio/x-psid");
    set("sit", "application/x-sit");
    set("skd", "application/x-koan");
    set("skm", "application/x-koan");
    set("skp", "application/x-koan");
    set("skt", "application/x-koan");
    set("sl", "application/x-seelogo");
    set("smi", "application/smil");
    set("smil", "application/smil");
    set("snd", "audio/basic");
    set("sol", "application/solids");
    set("spc", "application/x-pkcs7-certificates");
    set("spl", "application/futuresplash");
    set("spr", "application/x-sprite");
    set("sprite", "application/x-sprite");
    set("src", "application/x-wais-source");
    set("ssi", "text/x-server-parsed-html");
    set("ssm", "application/streamingmedia");
    set("sst", "application/vnd.ms-pki.certstore");
    set("step", "application/step");
    set("stl", "application/sla");
    set("stp", "application/step");
    set("sv4cpio", "application/x-sv4cpio");
    set("sv4crc", "application/x-sv4crc");
    set("svf", "image/vnd.dwg");
    set("svr", "application/x-world");
    set("swf", "application/x-shockwave-flash");
    set("t", "application/x-troff");
    set("talk", "text/x-speech");
    set("tar", "application/x-tar");
    set("tbk", "application/toolbook");
    set("tcl", "application/x-tcl");
    set("tcsh", "text/x-script.tcsh");
    set("tex", "application/x-tex");
    set("texi", "application/x-texinfo");
    set("texinfo", "application/x-texinfo");
    set("text", "text/plain");
    set("tgz", "application/gnutar");
    set("tif", "image/tiff");
    set("tiff", "image/tiff");
    set("tr", "application/x-troff");
    set("tsi", "audio/tsp-audio");
    set("tsp", "application/dsptype");
    set("tsv", "text/tab-separated-values");
    set("turbot", "image/florian");
    set("txt", "text/plain");
    set("uil", "text/x-uil");
    set("uni", "text/uri-list");
    set("unis", "text/uri-list");
    set("unv", "application/i-deas");
    set("uri", "text/uri-list");
    set("uris", "text/uri-list");
    set("ustar", "application/x-ustar");
    set("uu", "application/octet-stream");
    set("uue", "text/x-uuencode");
    set("vcd", "application/x-cdlink");
    set("vcs", "text/x-vcalendar");
    set("vda", "application/vda");
    set("vdo", "video/vdo");
    set("vew", "application/groupwise");
    set("viv", "video/vivo");
    set("vivo", "video/vivo");
    set("vmd", "application/vocaltec-media-desc");
    set("vmf", "application/vocaltec-media-file");
    set("voc", "audio/voc");
    set("vos", "video/vosaic");
    set("vox", "audio/voxware");
    set("vqe", "audio/x-twinvq-plugin");
    set("vqf", "audio/x-twinvq");
    set("vql", "audio/x-twinvq-plugin");
    set("vrml", "application/x-vrml");
    set("vrt", "x-world/x-vrt");
    set("vsd", "application/x-visio");
    set("vst", "application/x-visio");
    set("vsw", "application/x-visio");
    set("w60", "application/wordperfect6.0");
    set("w61", "application/wordperfect6.1");
    set("w6w", "application/msword");
    set("wav", "audio/wav");
    set("wb1", "application/x-qpro");
    set("wbmp", "image/vnd.wap.wbmp");
    set("web", "application/vnd.xara");
    set("wiz", "application/msword");
    set("wk1", "application/x-123");
    set("wmf", "windows/metafile");
    set("wml", "text/vnd.wap.wml");
    set("wmlc", "application/vnd.wap.wmlc");
    set("wmls", "text/vnd.wap.wmlscript");
    set("wmlsc", "application/vnd.wap.wmlscriptc");
    set("word", "application/msword");
    set("wp", "application/wordperfect");
    set("wp5", "application/wordperfect");
    set("wp5", "application/wordperfect6.0");
    set("wp6", "application/wordperfect");
    set("wpd", "application/wordperfect");
    set("wpd", "application/x-wpwin");
    set("wq1", "application/x-lotus");
    set("wri", "application/mswrite");
    set("wrl", "application/x-world");
    set("wrz", "model/vrml");
    set("wsc", "text/scriplet");
    set("wsrc", "application/x-wais-source");
    set("wtk", "application/x-wintalk");
    set("xbm", "image/x-xbitmap");
    set("xdr", "video/x-amt-demorun");
    set("xgz", "xgl/drawing");
    set("xif", "image/vnd.xiff");
    set("xl", "application/excel");
    set("xla", "application/excel");
    set("xlb", "application/excel");
    set("xlc", "application/excel");
    set("xld", "application/excel");
    set("xlk", "application/excel");
    set("xll", "application/excel");
    set("xll", "application/x-excel");
    set("xlm", "application/excel");
    set("xls", "application/excel");
    set("xlt", "application/excel");
    set("xlv", "application/excel");
    set("xlw", "application/x-msexcel");
    set("xm", "audio/xm");
    set("xml", "text/xml");
    set("xmz", "xgl/movie");
    set("xpix", "application/x-vnd.ls-xpix");
    set("xpm", "image/xpm");
    set("x-png", "image/png");
    set("xsr", "video/x-amt-showrun");
    set("xwd", "image/x-xwindowdump");
    set("xyz", "chemical/x-pdb");
    set("z", "application/x-compressed");
    set("zip", "application/zip");
    set("zoo", "application/octet-stream");
    set("zsh", "text/x-script.zsh");
  }
}

std::string MIMEType::get(const std::string& filext){
  Regex reg("^([^.]*)\\.(.*)$");
  std::string ext;
  reg.match(filext);
  ext = reg.get(1);
  
  std::map<std::string, std::string>::iterator it = exts.find(ext);
  if(it != exts.end())
    return it->second;
  else
    return default_type;
}

void MIMEType::set(const std::string& ext, const std::string& type){
  std::map<std::string, std::string>::iterator it = exts.find(ext);
  if(it != exts.end())
    exts.erase(it);

  exts.insert(std::pair<std::string, std::string>(ext, type));
}
