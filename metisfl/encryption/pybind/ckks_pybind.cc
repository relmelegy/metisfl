#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "metisfl/encryption/palisade/ckks_scheme.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)
namespace py = pybind11;

// We need to define CKKS as public in order
// to access its methods through PyBind.
class CKKSWrapper : public CKKS {

 public:
  ~CKKSWrapper() = default;
  CKKSWrapper(uint32_t batch_size, uint32_t scaling_factor_bits)
      : CKKS(batch_size, scaling_factor_bits) {}

  py::dict PyGetCryptoParamsFiles() {
    py::dict py_dict_crypto_params_files;
    auto crypto_params_files = CKKS::GetCryptoParamsFiles();
    py_dict_crypto_params_files["crypto_context_file"] = crypto_params_files.crypto_context_file;
    py_dict_crypto_params_files["public_key_file"] = crypto_params_files.public_key_file;
    py_dict_crypto_params_files["private_key_file"] = crypto_params_files.private_key_file;
    py_dict_crypto_params_files["eval_mult_key_file"] = crypto_params_files.eval_mult_key_file;
    return py_dict_crypto_params_files;
  }

  py::bytes PyEncrypt(py::array_t<double> data_array) {
    auto data_vec = std::vector<double>(
      data_array.data(), data_array.data() + data_array.size());
    auto data_encrypted_str = CKKS::Encrypt(data_vec);
    py::bytes py_bytes(data_encrypted_str);
    return py_bytes;
  }

  py::bytes PyComputeWeightedAverage(py::list learners_data,
                                     py::list scaling_factors) {

    if (learners_data.size() != scaling_factors.size()) {
      PLOG(FATAL) << "Error: learner_data and scaling_factors size need to match";
    }

    // Simply cast the given list of data and scaling factors to
    // their corresponding std::string and std::float vectors.
    auto learners_data_vec =
      learners_data.cast<std::vector<std::string>>();
    auto scaling_factors_vec =
      scaling_factors.cast<std::vector<float>>();

    auto weighted_avg_str = CKKS::ComputeWeightedAverage(
        learners_data_vec, scaling_factors_vec);
    py::bytes py_bytes_weighted_avg(weighted_avg_str);
    return py_bytes_weighted_avg;
  }

  py::array_t<double> PyDecrypt(string data,
                                unsigned long int data_dimensions) {
    auto data_decrypted = CKKS::Decrypt(data, data_dimensions);
    // Cast and release created vector.
    auto py_array_decrypted =
      py::array_t<double>(py::cast(std::move(data_decrypted)));
    return py_array_decrypted;
  }

};

PYBIND11_MODULE(fhe, m) {
  m.doc() = "CKKS soft python wrapper.";
  py::class_<CKKSWrapper>(m, "CKKS")
  .def(py::init<int, int>(),
      py::arg("batch_size"),
      py::arg("scaling_factor_bits"))
  .def("gen_crypto_context_and_keys", &CKKS::GenCryptoContextAndKeys)
  .def("get_crypto_params_files", &CKKSWrapper::PyGetCryptoParamsFiles)
  .def("load_crypto_context_from_file", &CKKS::LoadCryptoContextFromFile)
  .def("load_private_key_from_file", &CKKS::LoadPrivateKeyFromFile)
  .def("load_public_key_from_file", &CKKS::LoadPublicKeyFromFile)
  .def("load_context_and_keys_from_files", &CKKS::LoadContextAndKeysFromFiles)
  .def("encrypt", &CKKSWrapper::PyEncrypt)
  .def("compute_weighted_average", &CKKSWrapper::PyComputeWeightedAverage)
  .def("decrypt", &CKKSWrapper::PyDecrypt);

  m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: cmake_example
        .. autosummary::
           :toctree: _generate
    )pbdoc";

  #ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
  #else
  m.attr("__version__") = "dev";
  #endif
}
