class PrintWriter {

    byte buffer[] = new byte[1 << 16];
    int pos = 0;
    OutputStream os;

    PrintWriter(OutputStream os) {
        this.os = os;
    }

    void clearBuffer() {
        if (pos > 0) {
            try {
                os.write(buffer, 0, pos);
            } catch (IOException e) {
                throw new Error(e);
            }
            pos = 0;
        }
    }

    void flush() {
        clearBuffer();
        try {
            os.flush();
        } catch (IOException e) {
            throw new Error(e);
        }
    }

    void putChar(char c) {
        buffer[pos] = (byte)c;
        pos++;
        if (pos == buffer.length) {
            clearBuffer();
        }
    }

    void println(long value) {
        long pow = 1;
        while (value / pow > 9) {
            pow *= 10;
        }
        while (pow > 0) {
            putChar((char)('0' + value / pow));
            value %= pow;
            pow /= 10;
        }
        putChar('\n');
    }
}
