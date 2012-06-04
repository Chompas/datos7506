Ejemplo de uso:

OPCION 1: INSERTAR INCIDENTE

Los registros a ingresar tienen que tener el formato linea;hora;falla;accidente;formacion

Detalle -

Linea: La línea tiene que ser una cadena de caracteres que exista en el archivo lineas.txt
Hora: El formato de hora debe ser AAAA-MM-DTHH:MM:SS
Falla: La falla tiene que ser una cadena de caracteres que exista en el archivo fallas.txt
Accidente: El accidente tiene que ser una cadena de caracteres que exista en el archivo accidentes.txt
Formacion: Debe ser un numero entero

Se pueden insertar incidentes de a uno, presionando la tecla enter. 
Para finalizar la inserción se debe ingresar un asterisco y presionar enter

Ejemplo de incidente: Mitre;2011-10-2T12:12:45;no_cierra_puertas;incendio;15

OPCION 2: BUSCAR INCIDENTE

La búsqueda utiliza el mismo formato que en la inserción o se pueden utilizar comodines ingresando un guión en el campo deseado.

Ejemplos:
Buscar todos los incidentes de la línea Mitre: Mitre;-;-;-;-
Buscar todas las fallas de no_cierra_puertas: -;-;no_cierra_puertas;-;-
Buscar todos los incidentes de la línea Mitre de la formacion 2: Mitre;-;-;-;2

Luego de ingresar la cadena a buscar se preguntará si se desea hacer una búsqueda por rango de fechas.
En caso de elegir "s" se deberá ingresa el Horario Inicial y el Final con el formato de fecha arriba especificado.
Si se elige esta opción es indistinto si en la cadena de búsqueda se ingresó un horario o no ya que no sera tenido en cuenta.

La búsqueda devolverá los registros correspondietes a la cadena (si es que existen).


