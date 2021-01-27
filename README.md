# NodeMCU ile Kaza Detektörü

## Youtube Linki
[Link](https://youtu.be/bncMeatnMLU)

## Proje Tanımı
Günümüz koşullarında artan nüfus ile beraber trafiğe çıkan araç sayısı da artmış, bu da araçlarla yapılan kaza sayılarını gün geçtikçe etkilemeye devam etmiştir. Kullanıcı, araç veya yoldan kaynaklı karayolu ile yapılan yolculuk veya taşımalarda çokça kaza meydana gelmektedir. Kazalar gerçekleştiğinde büyük hasar durumlarında ambulans ve polis ihtiyacı doğabiliyor. Çevrede kazayı gören insanlar 112 ve polisi arayıp bilgilendiriyorlar. Kazalar sırasında kazazedeler için geçen bir saniyenin bile oldukça hayati önemi vardır, ilk yardımın erken ve doğru yere gelmesi önem taşımaktadır. Projenin çözmeye çalıştığı problem bu süreyi en aza indirerek en doğru konum bilgileriyle ilgili kişilere kaza yapıldığı an kaza olduğunun haberinin gitmesini sağlamaktır. Örnek olarak aracımız kimsesiz bir yerde uçurumdan aşağı düşerek kaza yaptığını düşünelim. Bu gerçekleştirmiş olduğum elektronik sistem, insan gereksinimi olmadan kaza durumunu araç sahibinin isteğine göre belirlediği kişilere, ailesine, şirketine, polise ve 112 acile anlık olarak bildirebilir. Bu şekilde 112 ve polis olay yerine kaza anında çevredeki insanların aramasını beklemeden ve yanlış bilgilerden uzak bir şekilde hızlı sürede erişme şansına sahip olur. Böylece erken müdahale edilme imkânı doğar ve yaralının iyileşme ihtimali artar. Mobil uygulama üzerinden sürücünün belirlediği kişiler aracın anlık sıcaklık, hız, konum gibi bilgilerine erişilebilir. Kaza yapıldığında 112 veya polise bir bilgi göndermek yerine mail yoluyla kazazedenin aile bireyine haber veriliyor. Üzerinde geliştirilmeler yapıldığı taktirde 112 ve polis alt yapısıyla birleştirilerek gerçek hayata adapte edilebilir ve piyasaya sürülebilir.

## Sistem Mimarisi
![enter image description here](https://i.imgur.com/FCtMpdn.png)


## Big Data ve Proje

Projemizde şu an bir cihaz aktif halde ama gerçek şartlar altında kullanıldığında Türkiye şartlarında milyonlarca araba mevcut. Bu araçlardan gelen verileri anlık olarak yönetmek için bu bilgileri tutacak bir platforma ve anlamlandıracak big data araçlarına ihtiyacımız var. Bu büyük verilerden istatistiksel olarak bazı anlamlar çıkarılabilir. Örnek olarak gps sensöründen gelen enlem ve boylam bilgileri anlamlandırılarak hangi lokasyon noktalarında kaza olduğunu, hangi hız ve hangi şartlarda gerçekleştiğini anlamlandırarak burada inceleme yapılabilir ve önlemler alınabilir. Verileri tutmak için noSQL olarak geçen MongoDB’yi tercih ederdim. IOT uygulamaları genel anlamda sensörler aracılığıyla büyük hacimli veriler işler bu nedenle sistemin hızlı şekilde ölçeklenmesi gerekir. MongoDB tercih etme sebebi öncellikle bir koleksiyondaki belgelerin farklı alanlara ve yapılara sahip olmasına imkân tanıyan dinamik şema tasarımını desteklemesi. Bu sayede yapı değiştiği zaman gereksinimleri karşılamak için yapıyı tekrardan oluşturmak zorunda kalınmıyor. Aynı zamanda gecikmesiz gerçek zamanlı analiz imkanına ve otomatik yük dengeleme fonksiyonuna sahiptir. Aktif olarak sensörlerin davranışlarından haberdar olmamız gerektiği için veri akış platformu olarak Apache kafka tercih ederim çünkü mongoDB ile kolay entegre edilebiliyor ve mongoDB sistemi kapalı olsa bile bir süre kafka bu veriyi tutma imkânı sağlıyor. Bu da sistemdeki mesaj ve veri kaybını da önlemekte. Büyük veriyi diğer sistemlere taşırken paralel çalışabilen ölçeklenebilir bir sistem olması da ek kolaylık sağlıyor.

## Flow Chart
![enter image description here](https://i.imgur.com/O09awcn.png)


# KULLANILAN TEKNOLOJİLER

## NodeMCU Lolin ESP8266 Wifi Geliştirme Kartı
NodeMCU, üzerindeki bağlantı noktalarını kullanarak bağlanılan elektronik devreleri ve bileşenleri yönetebileceğimiz programlanabilir, internete bağlanabilen minik bir elektronik geliştirme kitidir. Barındırdığı kablosuz ağa bağlanma özelliği ile projeyi uzak sunucuya bağlanarak veri transferi yapabilir. Arduino arayüzü üzerinden geliştirilme yapılmasına imkân sağlar. Bu projede NodeMCU, üzerine bağlı olan elektronik bileşenleri yazılımsal olarak yönetme ve elde ettiği verilerle karar vermede, verileri ThingSpeak, Firebase ve IFTTT platformlarına gönderme gibi işlemlerde merkezi rol almaktadır. Üzerindeki 3V çıkışları sayesinde elektronik bileşenlere güç sağlar. Dijital ve analog pinlere sahiptir. Bu projede GPS verisi, Ateş verisi ve Jiroskop verileri dijital pinlerle alınırken sıcaklık bilgisi analog pini ile alınmıştır.

## MPU6050 6 Eksenli Gyro ve Eğim Sensörü

Bu projenin en önemli bileşenlerinden olan MPU-6050 çeşitli hobi, multicopter ve robotik projelerinde sıklıklı kullanılan üzerinde 3 eksenli bir gyro ve 3 eksenli bir açısal ivme ölçer bulunduran 6 eksenli bir IMU sensör kartıdır. Bu projede her an arabanın hareketini tespit ederek serbest düşme, çarpma, büyük ölçüde sarsılma gibi durumların tespiti amacıyla kullanılmıştır ve kalibre edilmiştir. MPU6050 I2C protokolüyle haberleşen bir sensördür. Bu sebeple NodeMCU’nun I2C özelliğini kullanılması gerekir. Arduino içinde hazır olan “Wire.h” kütüphanesini kullanarak dışardan kütüphane kullanmaya gerek kalmadan MPU6060 üzerinden veriler alındı ve alınan Jiroskop ve İvmeölçer verilerine göre karar mekanizması geliştirildi.

## NEO-6M GPS Modülü

Diğer en önemli bileşen olan GY-NEO6 GPS modülü üzerinde bulunan NEO6 IC ile uçuş kontrol sistemleri başta olmaz üzere birçok projede GPS kullanarak konum kontrolü için kullanılır. 5 metrelik hassasiyete sahiptir.  
UART (RX-TX) haberleşme birimi kullanır. NodeMCU üzerinde RX ve TX haberleşmesini ayarlamak için “SoftwateSerial” kütüphanesi kullanıldı. NodeMCU üzerindeki GPIO12 ve GPIO13 pinleri bu kütüphane yardımıyla RX ve TX için ayarlandı. “TinyGPS” kütüphanesi yardımıyla enlem, boylam ve hız bilgileri kolaylıkla alınabiliyor. Bu projede modülün görevi her 5 saniyede bir hız, enlem ve boylam bilgilerini almaktır. Aynı zamanda kaza anında konum bilgisini de alarak kullanıcıya gönderilmesi amaçlanmıştır

## Ateş Algılayıcı Sensör

Ateş algılayıcı sensör kartı 760 nm- 1100 nm arasındaki dalga boyuna sahip ateşi tespit etmek için kullanılan bir sensör kartıdır. Bu projede araç kaza yaptığı durumda ateşi algılamak amacıyla koyulmuştur. Örnek olarak kaza durumunda sistem ateşi algıladığı anda itfaiyeyi hızlı bir şekilde çağırabilir


## LM35 Sıcaklık Sensörü

LM35 sıcaklık sensör devresi sayesinde bulunduğu ortamın sıcaklığını ölçmektedir. Ölçtüğü değeri santigrat derece (°C) ye çevirmek için ders dokümanlarında olan formül kullanıldı. Bu değer anlık olarak mobil uygulama üzerinden görülebilir.

## Firebase

Google’ın gerçek zamanlı veri depolama özelliği olan bulut tabanlı platformudur. NoSQL veritabanı kullanarak kullanıcılarla cihazlar arasında verileri gerçek zamanlı olarak saklayan ve senkronize eder. JSON veri formatını kullanır. Güncellenmiş veri, bağlı cihazlar arasında milisaniyeler içinde senkronize edilir ve uygulamamız çevrimdışı durumdaysa veriler saklanır ve ağ bağlantısı olduğunda senkronize edilir. Gmail hesabı ile herhangi bir yazılım yükleme ihtiyacı olmadan kullanılabilir. Bu projede NodeMCU 10 saniyede bir Firebase platformuna veri gönderiyor. Firebase kullanılmasının sebebi mobil uygulama yaparken Firebase’in kullanım kolaylığı sağlaması. Bu şekilde mobil uygulamada veriler anlık olarak gösterilebiliyor.

## ThingSpeak

Mathworks altyapısına sahip ThingSpeak, IoT bulut platformları içerisinde grafiksel sunum özellikleri ile öne çıkmaktadır. API desteği vardır. ThingSpeak’te veri iletişimi (gönderme ve alma işlemleri) kanallar aracılığıyla gerçekleştirilir. Public ve Private seçenekleri ile kanallar üzerinden iletişim gizli/güvenli ya da herkese açık yapılabilir. Bu projede thingspeak hız, sıcaklık, enlem ve boylam bilgileri kanallarda tutulmaktadır.

Kanal Linki: [https://thingspeak.com/channels/1257763](https://thingspeak.com/channels/1257763)

## IFTT

Bu platformun açılımı **“If This Then That”**, anlamı ise _“Bu Olursa Bunu Yap”_ demek. Bu platform mail, sms, tweet atma gibi bir sürü özelliğe sahip. Bu projede kaza yapıldığı zaman NodeMCU, IFTTT de kaza anı için oluşturulan webhooks eventini enlem ve boylam parametrelerini göndererek tetikliyor ve bu sayede kullanıcıya konum linkini içeren mail gitmesi sağlanıyor.

# Uygulama Görselleri
![enter image description here](https://i.imgur.com/2Oligof.png)

Kaza Öncesi Android Uygulaması
![enter image description here](https://i.imgur.com/dnTbTk2.png)

Kaza algılandığında Windowsda görünen Arduino Seri Port Ekranı
![enter image description here](https://i.imgur.com/o2Dy8aZ.png)

Kaza Sonrası Android Uygulaması
![enter image description here](https://i.imgur.com/aZfYN6g.png)

Gelen Google Maps Linki
![enter image description here](https://i.imgur.com/L4ZUQmi.png)

