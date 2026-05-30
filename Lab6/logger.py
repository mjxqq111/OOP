# -*- coding: utf-8 -*-

class Logger:
    def __init__(self, method, file=''):
        if method.lower() not in ['screen', 'file']:
            raise ValueError('Метод указан неверно!')

        if method.lower() == 'file' and len(file) == 0:
            raise TypeError('Для метода file должен быть указан путь к файлу!')

        self.method = method.lower()
        self.file = file

    def print_log(self, date, message):
        if self.method == 'screen':
            print('{} {}'.format(date, message))
        else:
            with open(self.file, 'a') as f:
                f.write('{} {}'.format(date, message))

    def clear_logs(self):
        open(self.file, 'w').close()