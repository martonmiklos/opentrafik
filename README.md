

OpenTrafik
================================


For international users: this software is designed mainly for warehouse managment in the Hungarian tobacco shops, so no English documentation provided. Sorry. 


Az OpenTrafik egy nyílt forráskódú trafikszoftver, amely képes napi szintű készletnyilvántartásra, és /* ha egyszer az NDN Zrt. publikálja a specifikációt akkor */ online napi jelentések továbbítására.

A szoftver C++ nyelven íródott a Qt keretrendszer segítségével. 

A legtöbb ilyen célú megoldással ellentétben ez nem egy kasszarendszer hanem egy PC-n futó szoftver amely egy vonalkódolvasóval ellátott pénztárgépből olvassa ki a napi forgalmat. 

Jelenleg egy péntárgépet támogat: a WisDor CR-681-et. Mivel e típus protokollja nem nyílt, ezért az adatok áttöltése a gyártó által biztosított segédprogrammal történik, majd a háttérben futó QTrafik az áttöltött adatfájlokból automatikusan kiolvassa a forgalmat. 

Tekintve, hogy a klasszikus pénztárgépeket hamarosan le kell majd cserélni AEE-vel felszerelt gépekre, ezért egy új péénztárgéptípus támogatása várható az Elcom EURO-150 Flexy-é. A gép protokollja nyitott, így nem szükséges egyéb program használata a forgalmi adataok letöltéséhez. 

Főbb képességei:
 * Raktárkészlet nyilvántartása
 * Bejövő számlák kezelése (fel kell vinni a bejövő számla számát illetve a végösszeget, így könnyen ellenőrízhető, hogy az összes tétel fel be lett-e vételezve)
 * Polccímke nyomtatás
 * Forgalmi jelentések (napi/heti/tetszőleges időtartam, bevétel/kiadás)
 * Termékforgalmi adatok
 * Az adatbázisban van egy 170 darabos terméklista vonalkódokkal, a jelenlegi árakkal
 * SQLite adatbázis az alapértelmezett, de lehetőség van MySQL illetve PostgreSQL használtatára is. Egyéb adatbázisok is támogathatók ld: https://qt-project.org/doc/qt-5.1/qtsql/sql-driver.html
 
A rendszer szabadon továbbfejleszthető, bővíthető. 