CC = gcc

objects = aux_tensor_product.o tensor_product.o

tensor: $(objects)
	$(CC) -pthread -o tensor $(objects)

aux_tensor_product.o: tensor.h

tensor_product.o: tensor.h

.PHONY: clean
clean:
	rm $(objects) tensor_mlaj.out tensor
