#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

#define MAX_ASCII_CODE 127
#define PRINTABLE_START_CODE 32
#define PRINTABLE_COUNT 95

enum Mode { ENCRYPT, DECRYPT };

auto shiftCipher(const Mode &mode, const std::string &str, const int &shift)
    -> std::string {
  std::vector<char> chr;
  chr.reserve(str.length());
  if (mode == Mode::ENCRYPT) {
    for (size_t i = 0; i < str.length(); i++) {
      char c = (str[i] + shift) % MAX_ASCII_CODE;
      chr[i] = c + (c < PRINTABLE_START_CODE ? PRINTABLE_START_CODE : 0);
    }
  } else if (mode == Mode::DECRYPT) {
    for (size_t i = 0; i < str.length(); i++) {
      char c = str[i] - shift;
      c = c - (c < PRINTABLE_START_CODE ? PRINTABLE_START_CODE : 0);
      chr[i] =
          c > 0 ? c : ((c % MAX_ASCII_CODE + MAX_ASCII_CODE) % MAX_ASCII_CODE);
    }
  }
  return std::string(chr.data());
}

auto main(int argc, char const *argv[]) -> int {
  namespace po = boost::program_options;

  po::options_description desc("ASCII Shift Cipher.\nUses all printable ASCII "
                               "Char Code from 32 to 126.\n\nOptions");
  desc.add_options()                                              //
      ("help,h", "Shows this message.")                           //
      ("shift,s", po::value<int>(), "Shift amount (arg >= 0).")   //
      ("encrypt,e", po::value<std::string>(), "Encrypt string.")  //
      ("decrypt,d", po::value<std::string>(), "Decrypt string."); //

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (const std::exception &e) {
    std::cout << desc << "\n";
    return 1;
  }
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 0;
  }

  if (!vm.count("shift")) {
    std::cout << desc << "\n";
    return 1;
  }

  const int shift = vm["shift"].as<int>();

  if (shift < 0) {
    std::cout << desc << "\n";
    return 1;
  }

  if (vm.count("encrypt")) {
    auto str = vm["encrypt"].as<std::string>();
    std::cout << shiftCipher(Mode::ENCRYPT, str, shift) << "\n";
  } else if (vm.count("decrypt")) {
    auto str = vm["decrypt"].as<std::string>();
    std::cout << shiftCipher(Mode::DECRYPT, str, shift) << "\n";
  } else {
    char printable[PRINTABLE_COUNT];
    for (size_t i = PRINTABLE_START_CODE; i < MAX_ASCII_CODE; i++) {
      printable[i - PRINTABLE_START_CODE] = i;
    }
    auto enc = shiftCipher(Mode::ENCRYPT, std::string(printable), shift);
    std::cout << "PLAIN\t"
              << "ENCODED\n"
              << "-----\t"
              << "-------\n";
    for (size_t i = 0; i < PRINTABLE_COUNT; i++) {
      std::cout << " " << printable[i] << "  \t   " << enc[i] << "   \n";
    }
  }

  return 0;
}
