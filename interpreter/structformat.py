from structure import DataBlock, StructField
import tokenize

BYTE = ('char', 'byte')
SHORT = ('short',)
INT = ('int',)
LONG = ('long',)
DATA_TYPES = BYTE + SHORT + INT + LONG
TYPE_MODIFIERS = ('unsigned', 'bend', 'lend')
TYPE_WORDS = DATA_TYPES + TYPE_MODIFIERS


class FormatParser:
    root = None
    current_element = None
    last_element = None
    current_type = None

    def parse_file(self, path):
        with open(path, 'rb') as file:
            tokens = tokenize.tokenize(file.readline)
            for t in tokens:
                toknum, tokval, _, _, _ = t
                print(t.exact_type, ":", tokval)
                self.process_token(toknum, tokval, t.exact_type)
        return self.root

    def process_token(self, toknum, tokval, exact_type):
        if toknum == tokenize.NAME:
            if tokval == 'struct':
                b = DataBlock()
                if self.current_element is not None:
                    self.current_element.add_child(b)
                self.current_element = b
            elif tokval in TYPE_WORDS:
                if self.current_type is None:
                    self.current_type = []
                self.current_type += tokval

                f = StructField(tokval, '')
                self.current_element.add_child(f)
                self.last_element = f
            else:
                # Имя переменной
                self.current_element.name = tokval
        elif toknum == tokenize.NEWLINE:
            self.current_type = None
            pass
        elif toknum == tokenize.ENCODING:
            pass


def read_str_file(path):
    parser = FormatParser()
    return parser.parse_file(path)
