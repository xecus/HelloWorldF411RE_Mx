"""
COM5のシリアルデータを標準出力にリダイレクトするスクリプト

使い方:
  python serial_to_stdout.py [--port COM5] [--baud 115200]

パイプ例:
  python serial_to_stdout.py | grep "H:"
  python serial_to_stdout.py > log.txt
"""

import serial
import sys
import argparse


def parse_args():
    parser = argparse.ArgumentParser(description="シリアルポートのデータを標準出力に流す")
    parser.add_argument("--port", default="COM5", help="シリアルポート (default: COM5)")
    parser.add_argument("--baud", type=int, default=115200, help="ボーレート (default: 115200)")
    return parser.parse_args()


def main():
    args = parse_args()

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
    except serial.SerialException as e:
        print(f"[ERROR] {e}", file=sys.stderr)
        sys.exit(1)

    ser.reset_input_buffer()

    try:
        while True:
            line_bytes = ser.readline()
            if not line_bytes:
                continue
            line = line_bytes.decode("ascii", errors="replace").rstrip("\r\n")
            print(line, flush=True)
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()


if __name__ == "__main__":
    main()
