class HeapMax:
	def __init__(self, f_cmp):
		self.f_cmp = f_cmp
		self.arr = []

	def __len__(self):
		return len(self.arr)

	def vacio(self):
		return len(self.arr) == 0

	def encolar(self, ele):
		self.arr.append(ele)
		self._upheap(len(self.arr) - 1)

	def desencolar(self):
		aux = self.arr.pop(0)
		self._downheap(0)
		return aux

	def _upheap(self, pos):
		if pos == 0: return

		padre = int(pos - 1 / 2)

		if self.f_cmp(self.arr[pos], self.arr[padre]) > 0:
			self.arr[padre], self.arr[pos] = self.arr[pos], self.arr[padre]
			self._upheap(padre)

	def _downheap(self, pos):
		if pos >= len(self): return

		padre = pos
		izq = 2 * pos + 1
		der = 2 * pos + 2

		if izq < len(self) and self.f_cmp(self.arr[izq], self.arr[padre]) > 0: padre = izq
		if der < len(self) and self.f_cmp(self.arr[der], self.arr[padre]) > 0: padre = der

		if padre != pos:
			self.arr[padre], self.arr[pos] = self.arr[pos], self.arr[padre]
			self._downheap(padre)