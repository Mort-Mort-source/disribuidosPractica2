import subprocess
import tkinter as tk
from tkinter import messagebox, simpledialog
import threading
import os

class CalculadoraRemota:
    def __init__(self, root):
        self.root = root
        self.root.title("Calculadora Remota (Frontend Python)")
        self.root.geometry("450x380")
        
        self.cliente_cpp = "./cliente"  # Nombre del ejecutable C++

        # Campo IP del servidor
        tk.Label(root, text="IP del Servidor:").grid(row=0, column=0, padx=5, pady=5, sticky="w")
        self.entry_ip = tk.Entry(root, font=("Arial", 12), width=20)
        self.entry_ip.grid(row=0, column=1, padx=5, pady=5)
        self.entry_ip.insert(0, "192.168.100.178")  # IP por defecto

        # Campos de números
        tk.Label(root, text="Número 1:").grid(row=1, column=0, padx=5, pady=5, sticky="w")
        self.entry1 = tk.Entry(root, font=("Arial", 14))
        self.entry1.grid(row=1, column=1, columnspan=3, sticky="we")
        self.entry1.bind("<Button-1>", lambda e: self.set_active(1))

        tk.Label(root, text="Número 2:").grid(row=2, column=0, padx=5, pady=5, sticky="w")
        self.entry2 = tk.Entry(root, font=("Arial", 14))
        self.entry2.grid(row=2, column=1, columnspan=3, sticky="we")
        self.entry2.bind("<Button-1>", lambda e: self.set_active(2))

        self.active = 1

        # Botones numéricos
        botones = ['7','8','9','4','5','6','1','2','3','0']
        row, col = 3, 0
        for btn in botones:
            tk.Button(root, text=btn, width=5, height=2,
                      command=lambda v=btn: self.agregar_digito(v)).grid(row=row, column=col, padx=2, pady=2)
            col += 1
            if col > 2:
                col = 0
                row += 1

        # Botones de operaciones
        tk.Button(root, text="+", width=5, height=2, command=lambda: self.enviar(1)).grid(row=3, column=3, padx=5)
        tk.Button(root, text="-", width=5, height=2, command=lambda: self.enviar(2)).grid(row=4, column=3, padx=5)
        tk.Button(root, text="*", width=5, height=2, command=lambda: self.enviar(3)).grid(row=5, column=3, padx=5)
        tk.Button(root, text="/", width=5, height=2, command=lambda: self.enviar(4)).grid(row=6, column=3, padx=5)

        tk.Button(root, text="C", width=5, height=2, command=self.limpiar).grid(row=6, column=0, padx=5)

        self.label_resultado = tk.Label(root, text="Resultado: ", font=("Arial", 12), fg="blue")
        self.label_resultado.grid(row=7, column=0, columnspan=4, pady=10)

    def set_active(self, n):
        self.active = n

    def agregar_digito(self, digito):
        if self.active == 1:
            self.entry1.insert(tk.END, digito)
        else:
            self.entry2.insert(tk.END, digito)

    def limpiar(self):
        self.entry1.delete(0, tk.END)
        self.entry2.delete(0, tk.END)
        self.label_resultado.config(text="Resultado: ")

    def enviar(self, op):
        try:
            num1 = float(self.entry1.get())
            num2 = float(self.entry2.get())
        except ValueError:
            messagebox.showerror("Error", "Ingrese números válidos")
            return

        ip_servidor = self.entry_ip.get().strip()
        if not ip_servidor:
            messagebox.showerror("Error", "Ingrese la IP del servidor")
            return

        self.label_resultado.config(text="Procesando...")
        threading.Thread(target=self._ejecutar_cliente_cpp, 
                        args=(ip_servidor, num1, num2, op), 
                        daemon=True).start()

    def _ejecutar_cliente_cpp(self, ip, num1, num2, op):
        try:
            resultado = subprocess.run(
                [self.cliente_cpp, ip, str(num1), str(num2), str(op)],
                capture_output=True,
                text=True,
                timeout=8
            )

            if resultado.returncode == 0:
                respuesta = resultado.stdout.strip()
                self.root.after(0, self._actualizar_resultado, respuesta)
            else:
                error = resultado.stderr.strip() or "Error en el cliente C++"
                self.root.after(0, self._mostrar_error, error)
        except FileNotFoundError:
            self.root.after(0, self._mostrar_error, f"No se encontró el cliente C++ ({self.cliente_cpp})")
        except subprocess.TimeoutExpired:
            self.root.after(0, self._mostrar_error, "Timeout: el cliente tardó demasiado")
        except Exception as e:
            self.root.after(0, self._mostrar_error, f"Error: {e}")

    def _actualizar_resultado(self, texto):
        self.label_resultado.config(text=texto)

    def _mostrar_error(self, msg):
        messagebox.showerror("Error", msg)
        self.label_resultado.config(text="Resultado: ")

if __name__ == "__main__":
    root = tk.Tk()
    app = CalculadoraRemota(root)
    root.mainloop()