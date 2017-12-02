double Rotate(double theeta,double theetadash)
{ 
  double  fi=theetadash-theeta;
  if(theeta>0&&theetadash<=-180+theeta)
    fi=360- theeta+ theetadash;
  if(theeta<0&&theetadash>180+theeta)
    fi=-360- theeta+ theetadash;
    return fi;
}

