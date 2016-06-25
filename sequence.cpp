#include <iostream>
#include <stdlib.h>
#include <cstring>

#define SIZE_INT sizeof(int)

using namespace std;

class SequenceGenerator
{
    public:
        SequenceGenerator(int *digits, int size) {
            this->digits = (int*) malloc(size * SIZE_INT);
			this->sizeResult = size > 0 ? 1 : 0;

            for (int i = 0; i < size; ++i) {
                this->digits[i] = digits[i];
                this->sizeResult *= (i + 1);
            }

            this->size = size;
            this->result = (int**) malloc(sizeof(int*) * this->sizeResult);
            this->curPosition = 0;
        };

        int** generate() {
            int *source;
            this->genSeq(source, this->digits, this->size);
            return this->result;
        };

		void setWidth(int width) {
			this->width = width;
		}

		int getCurrentPosition() {
			return this->curPosition;
		}

        int sizeResult;
	private:
		/**
		 * source - текущая последовательность
		 * digits - набор элементов для последовательности
		 * size - количество элементов в digits
		 */
	    void genSeq(int *source, int *digits, int size) {
			// длина текущей последовательности
			int sourceLength = this->size - size;

	        if (size == 0 || (this->width > 0 && sourceLength == this->width)) {
				// количество сравниваемых элементов
				int cmpSize = (this->width > 0 ? this->width : this->size);

				if (this->curPosition > 0) {
					bool test = true;

					for (int i = 0; i < cmpSize; ++i) {
						if (this->result[this->curPosition - 1][i] != source[i]) {
							test = false;
							break;
						}
					}

					if (test) {
						return;
					}
				}

	            this->result[this->curPosition] = source;
	            this->curPosition++;
	            return;
	        }

	        for (int i = 0; i < size; ++i) {
	            int *sourceNext = (int*) malloc((sourceLength + 1) * SIZE_INT);
	            int *distNext = (int*) malloc((size - 1) * SIZE_INT);

				memcpy(sourceNext, source, sourceLength * SIZE_INT);
				memcpy(sourceNext + sourceLength, digits + i, SIZE_INT);
				
				memcpy(distNext, digits, i * SIZE_INT);
				memcpy(distNext + i, digits + (i + 1), (size - i - 1) * SIZE_INT);

				if (size > 0 && (this->width <= 0 || sourceLength < this->width)) {
					/**
					 * Продолжаем генерировать последовательность когда:
					 * 1) Есть еще данные для последовательности (size > 0)
					 * AND
					 * 2.1) Ограничитель длины не установлен (this->width <= 0)
					 * OR
					 * 2.2.1) Установлен ограничитель длины (this->width > 0)
					 * 2.2.2) Построенная последовательность все еще меньше ограничителя (sourceLength < this->width)
					 */
	            	this->genSeq(sourceNext, distNext, size - 1);
				}

				if (size > 1 && (this->width <= 0 || sourceLength + 1 < this->width)) {
					free(sourceNext);
					free(distNext);
				}
	        }
	    };

	    int size;
		int *digits;
		int curPosition;
		int **result;
		int width;
};

int main(int argc, char* argv[]) {
	if (argc > 1) {
		char* firstVar = argv[1];
		int offset = 1;
		int width = 0;

		// -w (width)
		if (strlen(firstVar) == 2 && argc > 3 && (int) firstVar[0] == 45 && (int) firstVar[1] == 119) {
			width = atoi(argv[2]);
			offset += 2;
		}

		int *digits = (int*) malloc((argc - offset) * SIZE_INT);

		for (int i = 0; i < argc - offset; ++i) {
		    digits[i] = atoi(argv[i + offset]);
		}

		SequenceGenerator seq(digits, argc - offset);
		seq.setWidth(width);

		int **map = seq.generate();

		int sizeDataSet = (width > 0 ? width : argc - offset);

		for (int i = 0; i < seq.getCurrentPosition(); ++i) {
		    for (int j = 0; j < sizeDataSet; ++j) {
		        cout << map[i][j];
		        
		        if (j != argc - offset - 1) {
		        	cout << " ";
				}
		    }

		    cout << "\n";
			free(map[i]);
		}

		free(map);
		free(digits);
	} else {
		cout << "Not ok\n";
	}

	return 0;
}
