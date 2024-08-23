SOSA JOAQUIN - 45089031

Punto estrella: 

Encoding = 

// Fuente: https://www.techopedia.com/definition/948/encoding

Encoding es el proceso de conversion de datos hacia un formato requerido para diferentes propositos, incluyendo:

-Compilacion y ejecucion de un programa.
-Transmision de datos, comprension y descomprension, y almacenamiento
-Procesamiento de datos de aplicaciones, como la conversión de archivos.


// Fuente: CHATGPT
Para leer los dominios "raros" :

Esto se logra mediante un proceso llamado "Punycode", que convierte los nombres de dominio Unicode en una forma ASCII legible para máquinas y compatibles con el sistema DNS.

(
    - ASCII es un código en el cuál se representa cada carácter con un byte, en ASCII es posible representar 128 carácteres. 
    
    - Unicode es un código que asigna un número a cada carácter, tiene capacidad de codificar 1.1 millones de carácteres, es la codificación mas utilizada en la actualidad.
)

El proceso implica tomar los caracteres Unicode del nombre de dominio y convertirlos en una cadena ASCII, prefijada por "xn--". Por ejemplo, "中文.tw" se convertiría a "xn--fiq228c.tw" y "💩.la1" se convertiría a "xn--ls8h.la1".

Esto implicaria agregar al codigo principal =

from urllib.parse import urlparse

Luego en la funcion connect_server antes de la asignacion de "ip_address" :

if any(ord(char) >= 128 for char in server_name):
        parsed_url = urlparse('http://' + server_name)
        server_name = parsed_url.hostname
