#include <Rcpp.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include <ctime>

#include "model.h"
#include "infer.h"

using namespace std;

// [[Rcpp::export]]
SEXP btm(Rcpp::CharacterVector x, int K, int W, double alpha, double beta, int iter, int win = 15, bool background = false, int trace = 0) {
  Rcpp::Function format_posixct("format.POSIXct");
  Rcpp::Function sys_time("Sys.time");
  int save_step = 1;
  bool has_background = background;
  Rcpp::XPtr<Model> model(new Model(K, W, alpha, beta, iter, save_step, has_background), true);
  std::string line;
  for (int idx = 0; idx < x.size(); idx++){
    line = Rcpp::as<std::string>(x[idx]);
    Doc doc(line);
    doc.gen_biterms(model->bs, win);
    for (int i = 0; i < doc.size(); ++i) {
      int w = doc.get_w(i);
      model->pw_b[w] += 1;
    }
  }
  model->pw_b.normalize();
  model->model_init();
  for (int it = 1; it < iter + 1; ++it) {
    if(trace > 0){
      if ((it-1) % trace == 0){
        Rcpp::Rcout << Rcpp::as<std::string>(format_posixct(sys_time())) << " Start Gibbs sampling iteration " << it << endl;  
      }  
    }
    for (unsigned int b = 0; b < model->bs.size(); ++b) {
      model->update_biterm(model->bs[b]);
    }
    Rcpp::checkUserInterrupt();
  }
  // model->nb_z; n(b|z), size K*1
  // model->nwz;	n(w,z), size K*W
  // p(z) is determinated by the overall proportions of biterms in it
  Pvec<double> pz(model->nb_z); // nb_z: 
  pz.normalize(alpha);
  
  std::vector<double> p_z;
  for (unsigned int i = 0; i < pz.size(); ++i){
    p_z.push_back(pz[i]);
  }
  Rcpp::NumericMatrix pw_z(W, K); // p(w|z) = phi, size K * W
  for (int k = 0; k < K; k++) {
    for (int w = 0; w < W; w++){
      pw_z(w, k) = (model->nwz[k][w] + beta) / (model->nb_z[k] * 2 + W * beta);
    }
  }
  Rcpp::List out = Rcpp::List::create(
    Rcpp::Named("model") = model,
    Rcpp::Named("K") = K,
    Rcpp::Named("W") = W,
    Rcpp::Named("alpha") = alpha,
    Rcpp::Named("beta") = beta,
    Rcpp::Named("iter") = iter,
    Rcpp::Named("background") = background,
    Rcpp::Named("theta") = p_z,
    Rcpp::Named("phi") = pw_z
  );
  return out;
}

// [[Rcpp::export]]
Rcpp::NumericMatrix btm_infer(const Rcpp::List & model, Rcpp::CharacterVector x, std::string type) {
  int K = Rcpp::as<int>(model["K"]);
  int W = Rcpp::as<int>(model["W"]);
  Rcpp::NumericVector theta = Rcpp::as<Rcpp::NumericVector>(model["theta"]);
  Rcpp::NumericMatrix phi = Rcpp::as<Rcpp::NumericMatrix>(model["phi"]);
  Rcpp::NumericMatrix scores(x.size(), K);
  
  Pvec<double> pz(K);
  Pmat<double> pw_z(K, W);
  for (int i = 0; i < theta.size(); ++i){
    pz[i] = theta(i);
  }
  for (int k = 0; k < K; k++) {
    for (int w = 0; w < W; w++){
      pw_z[k][w] = phi(w, k);
    }
  }
  Infer inf(type, K);
  inf.pz = pz;
  inf.pw_z = pw_z;
  std::string line;
  for (int idx = 0; idx < x.size(); idx++){
    line = Rcpp::as<std::string>(x[idx]);
    Doc doc(line);
    Pvec<double> pz_d(K);
    inf.doc_infer(doc, pz_d);
    for (int k = 0; k < K; k++) {
      scores(idx, k) = pz_d[k];  
    }
  }
  return(scores);
}



// [[Rcpp::export]]
Rcpp::List btm_biterms(SEXP btm_model) {
  Rcpp::XPtr<Model> model(btm_model);
  unsigned int nr_biterms = model->bs.size();
  std::vector<int> term1;
  std::vector<int> term2;
  std::vector<int> topic;
  for (unsigned int i = 0; i < nr_biterms; i++){
    term1.push_back(model->bs[i].get_wi() + 1);
    term2.push_back(model->bs[i].get_wj() + 1);
    topic.push_back(model->bs[i].get_z() + 1);
  }
  Rcpp::List out = Rcpp::List::create(
    Rcpp::Named("n") = nr_biterms,
    Rcpp::Named("biterms") = Rcpp::List::create(
      Rcpp::Named("term1") = term1,
      Rcpp::Named("term2") = term2,
      Rcpp::Named("topic") = topic
    )
  );
  return out;
}
