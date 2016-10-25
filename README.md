# BatchGUI
![BatchGUI screenshot](http://www.glusoft.com/data/img/BatchGUI.png)
[![Build Status](https://travis-ci.org/NicolasPerdu/BatchGUI.svg?branch=master)](https://travis-ci.org/NicolasPerdu/BatchGUI)

## Examples

Compress PNG images files using three program :
**Input Img -> TruePNG -> zoflipng -> deflopt -> Output Img**

```xml
<?xml version="1.0" encoding="utf-8"?>
<root>
	<prog out="examples/result/$FILE$-new.png" exe="examples/bin/TruePNG" arg="/out $OUT$ /o max $IN$">
		<prog out="examples/result/$FILE$.png" exe="examples/bin/zopflipng" arg="-y -m 
		--filters=01234mepb --lossy_transparent $IN$ $OUT$">
			<prog out="examples/result/$FILE$.png" exe="examples/bin/deflopt" arg="$IN$" />
		</prog>
	</prog>
</root>
```

Another example where you can generate multiple output images with one images. 
In this case one image is compressed with TruePNG and the other use zopflipng.

```xml
<?xml version="1.0" encoding="utf-8"?>
<root>
	<prog out="examples/result/$FILE$-new.png" exe="examples/bin/TruePNG" arg="/out $OUT$ /o max $IN$" />
	<prog out="examples/result/$FILE$-newzopfli.png" exe="examples/bin/zopflipng" arg="-y -m --filters=01234mepb $IN$ $OUT$"/>
</root>
```
