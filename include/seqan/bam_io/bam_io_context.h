// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2013, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================
// Class BamIOContext, accessor functions.
// ==========================================================================

#ifndef INCLUDE_SEQAN_BAM_IO_BAM_IO_CONTEXT_H_
#define INCLUDE_SEQAN_BAM_IO_BAM_IO_CONTEXT_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

/*!
 * @class BamIOContext
 * @headerfile <seqan/bam_io.h>
 * @brief The I/O context to use for BAM I/O.
 *
 * @signature template <typename TNameStore[, typename TNameStoreCache]>
 *            class BamIOContext;
 *
 * @tparam TNameStore      The name store class.
 * @tparam TNameStoreCache The name store cache class.  Defaults to @link NameStoreCache @endlink &lt;TNameStore&gtl;.
 *
 * @section Examples
 *
 * Creating a @link BamIOContext @endlink for a raw @link StringSet @endlink of @link CharString @endlink.
 *
 * @code{.cpp}
 * StringSet<CharString> nameStore;
 * NameStoreCache<StringSet<CharString> > nameStoreCache(nameStore);
 * BamIOContext<StringSet<CharString> > bamIOContext(nameStore, nameStoreCache);
 * // ...
 * @endcode
 *
 * Using a @link BamIOContext @endlink with a @link FragmentStore @endlink.
 *
 * @code{.cpp}
 * typedef FragmentStore<>::TContigNameStore         TNameStore;
 * typedef NameStoreCache<TNameStore>                TNameStoreCache;
 * FragmentStore<> store;
 * // Optionally, do something with store.
 * typedef BamIOContext<TNameStore, TNameStoreCache> TBamIOContext;
 * TBamIOContext bamIOContext(store.contigNameStore, store.contigNameStoreCache);
 * // ...
 * @endcode
 */

/*!
 * @fn BamIOContext::BamIOContext
 * @headerfile <seqan/bam_io.h>
 * @brief Constructor.
 *
 * @signature BamIOContext::BamIOContext();
 *
 * Only the default constructor is provided.
 */

/*!
 * @typedef BamIOContext::TNameStore
 *
 * @brief The name store class.

 * @signature typedef (...) BamIOContext::TNameStore;
 */

/*!
 * @typedef BamIOContext::TNameStoreCache
 * @brief The name store cache class.
 *
 * @signature typedef (...) BamIOContext::TNameStoreCache;
 */

template <
    typename TNameStore_ = StringSet<CharString>,
    typename TNameStoreCache_ = NameStoreCache<TNameStore_>,
    typename TStorageSpec = void>
class BamIOContext
{
public:
    typedef TNameStore_ TNameStore;
    typedef TNameStoreCache_ TNameStoreCache;
    typedef __int32 TSequenceLength;
    typedef String<TSequenceLength> TSequenceLengths;

    typedef typename If<IsSameType<TStorageSpec, void>,
                        Dependent<>, TStorageSpec>::Type TNSStorageSpec;
    typedef typename If<IsSameType<TStorageSpec, void>,
                        Owner<>, TStorageSpec>::Type TSLStorageSpec;

    typedef typename StorageSwitch<TNameStore, TNSStorageSpec>::Type        TNameStoreMember;
    typedef typename StorageSwitch<TNameStoreCache, TNSStorageSpec>::Type   TNameStoreCacheMember;
    typedef typename StorageSwitch<TSequenceLengths, TSLStorageSpec>::Type  TSequenceLengthsMember;

    TNameStoreMember        _nameStore;
    TNameStoreCacheMember   _nameStoreCache;
    TSequenceLengthsMember  _sequenceLengths;
    CharString              buffer;
    String<CharString>      buffers;
    String<unsigned>        translateFile2GlobalRefId;

    BamIOContext() :
        _nameStore(TNameStoreMember()),
        _nameStoreCache(ifSwitch(typename IsPointer<TNameStoreCacheMember>::Type(),
                                 (TNameStoreCache*)NULL,
                                 _nameStore)),
        _sequenceLengths(TSequenceLengthsMember())
    {}

    BamIOContext(TNameStore & nameStore_, TNameStoreCache & nameStoreCache_) :
        _nameStore(_referenceCast<typename Parameter_<TNameStoreMember>::Type>(nameStore_)),
        _nameStoreCache(ifSwitch(typename IsPointer<TNameStoreCacheMember>::Type(),
                                 &nameStoreCache_,
                                 _nameStore)),
        _sequenceLengths(TSequenceLengthsMember())
    {}

    template <typename TOtherStorageSpec>
    BamIOContext(BamIOContext<TNameStore, TNameStoreCache, TOtherStorageSpec> & other) :
        _nameStore(_referenceCast<typename Parameter_<TNameStoreMember>::Type>(nameStore(other))),
        _nameStoreCache(ifSwitch(typename IsPointer<TNameStoreCacheMember>::Type(),
                                 &nameStoreCache(other),
                                 _nameStore)),
        _sequenceLengths(_referenceCast<typename Parameter_<TSequenceLengthsMember>::Type>(sequenceLengths(other)))
    {}
};

// ============================================================================
// Metafunctions
// ============================================================================

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function nameStore()
// ----------------------------------------------------------------------------

/*!
 * @fn BamIOContext#nameStoreCache
 * @brief Return reference to name store cache from @link BamIOContext @endlink.
 *
 * @signature TNameStoreRef nameStoreCache(context);
 *
 * @param[in] context The @link BamIOContext @endlink to query.
 *
 * @return TNameStoreRef A reference to the <tt>TNameStore</tt> of the context.
 */

template <typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline TNameStore &
nameStore(BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> & context)
{
    return _referenceCast<TNameStore &>(context._nameStore);
}

template <typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline TNameStore const &
nameStore(BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> const & context)
{
    return _referenceCast<TNameStore const &>(context._nameStore);
}

template <typename TNameStore, typename TNameStoreCache>
inline void
setNameStore(BamIOContext<TNameStore, TNameStoreCache, Dependent<> > & context, TNameStore & nameStore)
{
    context._nameStore = &nameStore;
}

// ----------------------------------------------------------------------------
// Function sequenceLengths()
// ----------------------------------------------------------------------------

template <typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline typename BamIOContext<TNameStore, TNameStoreCache, TStorageSpec>::TSequenceLengths &
sequenceLengths(BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> & context)
{
    typedef typename BamIOContext<TNameStore, TNameStoreCache, TStorageSpec>::TSequenceLengths TSequenceLengths;
    return _referenceCast<TSequenceLengths &>(context._sequenceLengths);
}

template <typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline typename BamIOContext<TNameStore, TNameStoreCache, TStorageSpec>::TSequenceLengths const &
sequenceLengths(BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> const & context)
{
    typedef typename BamIOContext<TNameStore, TNameStoreCache, TStorageSpec>::TSequenceLengths TSequenceLengths;
    return _referenceCast<TSequenceLengths const &>(context._sequenceLengths);
}

template <typename TNameStore, typename TNameStoreCache, typename TSequenceLengths>
inline void
setSequenceLengths(BamIOContext<TNameStore, TNameStoreCache, Dependent<> > & context, TSequenceLengths & sequenceLengths)
{
    context._sequenceLengths = &sequenceLengths;
}

// ----------------------------------------------------------------------------
// Function nameStoreCache()
// ----------------------------------------------------------------------------

/*!
 * @fn BamIOContext#nameStore
 * @headerfile <seqan/bam_io.h>
 * @brief Return reference to name store from @link BamIOContext @endlink.
 *
 * @signature TNameStoreCacheRef nameStore(context);
 *
 * @param[in] context The @link BamIOContext @endlink to query.
 *
 * @return TNameStoreCacheRef A reference to the <tt>TNameStoreCache</tt> of the context.
 */

template <typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline TNameStoreCache &
nameStoreCache(BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> & context)
{
    return _referenceCast<TNameStoreCache &>(context._nameStoreCache);
}

template <typename TNameStore, typename TNameStoreCache, typename TStorageSpec>
inline TNameStoreCache const &
nameStoreCache(BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> const & context)
{
    return _referenceCast<TNameStoreCache const &>(context._nameStoreCache);
}

template <typename TNameStore, typename TNameStoreCache>
inline void
setNameStoreCache(BamIOContext<TNameStore, TNameStoreCache, Dependent<> > & context, TNameStoreCache & nameStoreCache)
{
    context._nameStoreCache = &nameStoreCache;
}

}  // namespace seqan

#endif  // #ifndef INCLUDE_SEQAN_BAM_IO_BAM_IO_CONTEXT_H_