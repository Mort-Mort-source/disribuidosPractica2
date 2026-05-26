# Instructivo de Ejecución y Uso del Sistema Cliente-Servidor

## Requisitos Previos

Antes de ejecutar el sistema asegúrese de tener instalado:

- Compilador C++ con soporte para C++11 o superior
- Python 3
- Biblioteca Tkinter
- Sistema operativo Linux o Windows

---

# Estructura General

El sistema está compuesto por:

- Servidor multihilo en C++
- Cliente TCP en C++
- Interfaz gráfica en Python

---

# Compilación del Servidor

Ubíquese en la carpeta del servidor y compile:

```bash
g++ servidor.cpp -o servidor -pthread
```

## Ejecutar el servidor

```bash
./servidor
```

El servidor comenzará a escuchar conexiones en el puerto:

```text
65432
```

---

# Compilación del Cliente C++

Ubíquese en la carpeta del cliente y compile:

```bash
g++ cliente.cpp -o cliente
```


---

# Ejecución del Frontend

Ejecute la interfaz gráfica con:

```bash
python3 frontend.py
```



---

# Flujo de Uso

1. Levantar el servidor C++.
2. Ejecutar el frontend Python.
3. Ingresar:
   - Dirección IP
   - Primer número
   - Segundo número
   - Operación
4. Presionar el botón de cálculo.
5. El frontend ejecutará el cliente C++.
6. El cliente enviará la solicitud al servidor.
7. El servidor procesará la operación.
8. El resultado será mostrado en pantalla.

---

# Formato del Protocolo TCP

El cliente envía mensajes en formato texto:

```text
num1,num2,operacion
```

Ejemplo:

```text
10,5,suma
```

---

# Posibles Errores

## Error de conexión

Verifique que:

- El servidor esté encendido
- La IP sea correcta
- El puerto 65432 esté disponible

---

## División entre cero

El servidor devolverá un mensaje de error.

---

## Operación inválida

Las operaciones deben coincidir exactamente con:

```text
suma
resta
multiplicacion
division
```

---

# Finalización

Para detener el servidor use:

```bash
CTRL + C
```

---