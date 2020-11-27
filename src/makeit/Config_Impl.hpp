template<typename L>
makeit::Exception<L>::Exception(const L &location, const Error err, const std::vector<ExceptionArg> &args) :
  location(location), err(err), args(args)
{
}

template<typename L>
const L makeit::Exception<L>::get_location() const
{
  return location;
}

template<typename L>
const makeit::Error makeit::Exception<L>::get_err() const
{
  return err;
}

template<typename L>
const std::vector<makeit::ExceptionArg>& makeit::Exception<L>::get_args() const
{
  return args;
}

template<typename L>
const char* makeit::Exception<L>::get_message() const
{
  const char* raw = get_error(err);

  char* msg = new char[2048];
  uint32_t pos = 0;
  uint32_t arg_pos = 0;

  while (*raw != '\0')
  {
    if (*raw == '%' && *(raw + 1) != '\0')
    {
      char type[3] {*raw, *((raw++) + 1), '\0'};
      pos += sprintf(msg + pos, type, args.at(arg_pos++));
      raw++;
      continue;
    }

    msg[pos++] = *(raw++);
  }

  return msg;
}
