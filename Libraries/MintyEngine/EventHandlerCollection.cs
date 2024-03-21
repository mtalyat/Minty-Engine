using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MintyEngine
{
    public class EventHandlerCollection<TKey, THandler, TArgs> where THandler : Delegate
    {
        public class HandlerWrapper
        {
            private THandler _handler;
            public THandler Handler
            {
                get => _handler;
                internal set => _handler = value;
            }

            internal HandlerWrapper() { }

            internal HandlerWrapper(THandler handler)
            {
                _handler = handler;
            }

            public static implicit operator HandlerWrapper(THandler handler) => new HandlerWrapper(handler);

            public static HandlerWrapper operator +(HandlerWrapper left, HandlerWrapper right)
            {
                return new HandlerWrapper((THandler)Delegate.Combine(left._handler, right._handler));
            }

            public static HandlerWrapper operator -(HandlerWrapper left, HandlerWrapper right)
            {
                return new HandlerWrapper((THandler)Delegate.Remove(left._handler, right._handler));
            }

            public object Invoke(params object[] args)
            {
                return _handler.DynamicInvoke(args);
            }
        }

        private Dictionary<TKey, HandlerWrapper> _events = new Dictionary<TKey, HandlerWrapper>();

        public THandler this[TKey key]
        {
            get
            {
                if (_events.TryGetValue(key, out HandlerWrapper handler))
                {
                    // event exists
                    return handler.Handler;
                }
                else
                {
                    // create new event
                    HandlerWrapper wrapper = new HandlerWrapper();
                    _events[key] = wrapper;
                    return wrapper.Handler;
                }
            }
            set
            {
                _events[key].Handler = value;
            }
        }

        public object Invoke(TKey key, params object[] args)
        {
            if (_events.TryGetValue(key, out HandlerWrapper handler))
            {
                return handler.Invoke(args);
            }

            return null;
        }
    }
}
