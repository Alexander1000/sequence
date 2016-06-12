#include <iostream>
#include <memory.h>

#define SIZE_INT sizeof(int)

using namespace std;

class SequenceGenerator
{
    public:
        SequenceGenerator(int *digits, int size) {
            this->digits = (int*) malloc(sizeof(int) * size);
            memset(this->digits, 0, sizeof(int) * size);

            if (size > 0) {
                this->sizeResult = 1;
            } else {
                this->sizeResult = 0;
            }

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

        int sizeResult;
	private:
	    void genSeq(int *source, int *digits, int size) {
	        if (size == 0) {
	            this->result[this->curPosition] = source;
	            this->curPosition++;
	            return;
	        }

	        for (int i = 0; i < size; ++i) {
	            int *sourceNext = (int*) malloc((this->size - size + 1) * SIZE_INT);
	            int *distNext = (int*) malloc((size - 1) * SIZE_INT);

				memcpy(sourceNext, source, (this->size - size) * SIZE_INT);
				memcpy(sourceNext + (this->size - size), digits + i, SIZE_INT);
				
				memcpy(distNext, digits, i * SIZE_INT);
				memcpy(distNext + i, digits + (i + 1), (size - i - 1) * SIZE_INT);

	            this->genSeq(sourceNext, distNext, size - 1);

				if (size > 1) {
					free(sourceNext);
					free(distNext);
				}
	        }
	    };

	    int size;
		int *digits;
		int curPosition;
		int **result;
};

int main(int argc, char* argv[]) {
	if (argc > 1) {
		int *digits = (int*) malloc((argc - 1) * sizeof(int));

		for (int i = 0; i < argc - 1; ++i) {
		    digits[i] = atoi(argv[i + 1]);
		}

		SequenceGenerator seq(digits, argc - 1);

		int **map = seq.generate();

		for (int i = 0; i < seq.sizeResult; ++i) {
		    for (int j = 0; j < argc - 1; ++j) {
		        cout << map[i][j] << " ";
		    }

		    cout << "\n";
			free(map[i]);
		}

		free(map);
	} else {
		cout << "Not ok\n";
	}

	return 0;
}
